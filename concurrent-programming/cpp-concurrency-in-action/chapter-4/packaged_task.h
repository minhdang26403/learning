#pragma once

#include "future.h"
#include "promise.h"
#include <exception>
#include <functional>
#include <future>
#include <optional>

template <typename> class PackagedTask; // not defined

template <typename R, typename... ArgTypes> class PackagedTask<R(ArgTypes...)> {
public:
  // Constructors
  PackagedTask() noexcept = default;
  template <typename F> explicit PackagedTask(F &&f) {
    // `std::function` requires captured variables to be copyable, but
    // `Promise` is not copyable, so we use `shared_ptr` here as a workaround
    task_ = [f = std::forward<F>(f)](Promise<R> promise, ArgTypes... args) {
      try {
        promise.set_value(f(std::forward<ArgTypes>(args)...));
      } catch (...) {
        promise.set_exception(std::current_exception());
      }
    };
  }
  PackagedTask(const PackagedTask &) = delete;
  PackagedTask(PackagedTask &&other) noexcept
      : task_(std::move(other.task_)), promise_(std::move(other.promise_)) {
    other.task_ = std::nullopt;
  }

  // Assignment operators
  PackagedTask &operator=(const PackagedTask &) = delete;
  PackagedTask &operator=(PackagedTask &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    PackagedTask temp{std::move(other)};
    temp.swap(*this);
    return *this;
  }

  bool valid() const noexcept { return task_.has_value(); }

  void swap(PackagedTask &other) noexcept {
    std::swap(task_, other.task_);
    std::swap(promise_, other.promise_);
  }

  Future<R> get_future() {
    if (!valid()) {
      throw std::future_error(std::future_errc::no_state);
    }
    return promise_.get_future();
  }

  void operator()(ArgTypes... args) {
    if (!valid()) {
      throw std::future_error(std::future_errc::no_state);
    }
    if (promise_already_satisfied_) {
      throw std::future_error(std::future_errc::promise_already_satisfied);
    }
    promise_already_satisfied_ = true;
    task_->operator()(std::move(promise_), std::forward<ArgTypes>(args)...);
  }

  void reset() {
    if (!valid()) {
      throw std::future_error(std::future_errc::no_state);
    }
    promise_ = Promise<R>{};
  }

private:
  std::optional<std::function<void(Promise<R>, ArgTypes...)>> task_;
  Promise<R> promise_;
  bool promise_already_satisfied_{};
};
