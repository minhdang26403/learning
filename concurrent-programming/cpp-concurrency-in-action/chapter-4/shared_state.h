#pragma once

#include <exception>
#include <mutex>

template <typename R> struct SharedState {
  R value_;
  std::exception_ptr exception_;
  bool ready_{};
  std::mutex mtx_;
  std::condition_variable cv_;
};
