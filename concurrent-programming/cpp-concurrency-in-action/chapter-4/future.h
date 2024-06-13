#pragma once
#include "shared_state.h"
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <future>
#include <mutex>

template <typename R> class Promise;
template <typename R> class SharedFuture;

template <typename R> class Future {
public:
  // Constructors
  Future() noexcept = default;
  Future(Future &&other) noexcept : state_(std::move(other.state_)) {}
  Future(const Future &) = delete;

  // Destructor
  // TODO: Destructor may block in case of `Future` returned from `Async`
  ~Future() = default;

  // Assignment operators
  Future &operator=(Future &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    state_ = std::move(other.state_);
    return *this;
  }
  Future &operator=(const Future &) = delete;

  SharedFuture<R> share() {
    return SharedFuture<R>{std::move(*this)};
  }

  R get() {
    wait();
    auto sp = std::move(state_);
    if (sp->exception_) {
      std::rethrow_exception(sp->exception_);
    }
    assert(!valid());

    return sp->value_;
  }

  bool valid() const noexcept { return state_ != nullptr; }

  void wait() const {
    if (!valid()) {
      throw std::future_error(std::future_errc::no_state);
    }
    std::unique_lock lock(state_->mtx_);
    state_->cv_.wait(lock, [this] { return state_->ready_; });
    assert(valid());
  }

  template <typename Rep, typename Period>
  std::future_status
  wait_for(const std::chrono::duration<Rep, Period> &timeout_duration) const {
    auto timeout_time = std::chrono::steady_clock::now() + timeout_duration;
    return wait_until(timeout_time);
  }

  template <typename Rep, typename Period>
  std::future_status
  wait_until(const std::chrono::time_point<Rep, Period> &timeout_time) const {
    // TODO: How to detect deferred status here? It is only relevant to
    // `Async`
    // This function returns immediately if `Future` is the result of a call to
    // `Async`
    if (!valid()) {
      throw std::future_error(std::future_errc::no_state);
    }
    std::unique_lock lock(state_->mtx_);
    auto status = std::cv_status::no_timeout;
    while (!state_->ready_ && status != std::cv_status::timeout) {
      status = state_->cv_.wait_until(lock, timeout_time);
    }
    return (status == std::cv_status::no_timeout) ? std::future_status::ready
                                                  : std::future_status::timeout;
  }

private:
  friend class Promise<R>;

  // Only friend class of `Future` can use this constructor
  Future(const std::shared_ptr<SharedState<R>> state) : state_(state) {}

  // Allows `SharedFuture` to access the state of `Future`
  friend class SharedFuture<R>;

  std::shared_ptr<SharedState<R>> state_;
};
