#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <deque>
#include <mutex>

#include "function_wrapper.h"

class work_stealing_queue {
 public:
  work_stealing_queue() = default;

  work_stealing_queue(const work_stealing_queue&) = delete;

  work_stealing_queue& operator=(const work_stealing_queue&) = delete;

  void push(function_wrapper data) {
    std::lock_guard<std::mutex> lock(the_mutex);
    the_queue.push_front(std::move(data));
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(the_mutex);
    return the_queue.empty();
  }

  bool try_pop(function_wrapper& res) {
    std::lock_guard<std::mutex> lock(the_mutex);
    if (the_queue.empty()) {
      return false;
    }
    res = std::move(the_queue.front());
    the_queue.pop_front();
    return true;
  }

  bool try_steal(function_wrapper& res) {
    std::lock_guard<std::mutex> lock(the_mutex);
    if (the_queue.empty()) {
      return false;
    }
    res = std::move(the_queue.back());
    the_queue.pop_back();
    return true;
  }

 private:
  std::deque<function_wrapper> the_queue;
  mutable std::mutex the_mutex;
};



#endif  // THREAD_POOL_H_