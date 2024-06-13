#pragma once

#include "future.h"
#include "shared_state.h"
#include <future>

template <typename R> class Promise {
public:
  // Constructors
  Promise() : state_(std::make_shared<SharedState<R>>()) {}
  Promise(Promise &&other) noexcept : state_(std::move(other.state_)) {}
  Promise(const Promise &) = delete;

  // Destructor
  ~Promise() {
    if (state_ == nullptr) {
      return;
    }
    std::scoped_lock lock(state_->mtx_);
    if (state_->ready_) {
      return;
    }
    state_->exception_ = std::make_exception_ptr(
        std::future_error(std::future_errc::broken_promise));
    state_->ready_ = true;
    state_->cv_.notify_one();
  }

  // Assignment operators
  Promise& operator=(Promise&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    Promise temp{std::move(other)};
    temp.swap(*this);
    return *this;
  }

  Promise& operator=(const Promise& rhs) = delete;

  void swap(Promise& other) noexcept {
    std::swap(state_, other.state_);
    std::swap(future_already_retrieved_, other.future_already_retrieved_);
  }  

  Future<R> get_future() {
    if (state_ == nullptr) {
      throw std::future_error(std::future_errc::no_state);
    }
    if (future_already_retrieved_) {
      throw std::future_error(std::future_errc::future_already_retrieved);
    }
    future_already_retrieved_ = true;
    return Future<R>(state_);
  }

  void set_value(R value) {
    if (state_ == nullptr) {
      throw std::future_error(std::future_errc::no_state);
    }
    if (state_->ready_) {
      throw std::future_error(std::future_errc::promise_already_satisfied);
    }
    state_->value_ = std::move(value);
    make_ready();
  }

  void set_exception(std::exception_ptr p) {
    if (state_ == nullptr) {
      throw std::future_error(std::future_errc::no_state);
    }
    if (state_->ready_) {
      throw std::future_error(std::future_errc::promise_already_satisfied);
    }
    state_->exception_ = p;
    make_ready();
  }

private:
  void make_ready() {
    std::scoped_lock lock(state_->mtx_);
    state_->ready_ = true;
    state_->cv_.notify_all();
  }

  std::shared_ptr<SharedState<R>> state_;
  bool future_already_retrieved_{};
};
