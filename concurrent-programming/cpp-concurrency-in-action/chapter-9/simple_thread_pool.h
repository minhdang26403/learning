#ifndef SIMPLE_THREAD_POOL_H_
#define SIMPLE_THREAD_POOL_H_

#include <atomic>
#include <functional>
#include <thread>
#include <vector>

#include "../chapter-6/fine_grained_queue.h"
#include "join_threads.h"

class thread_pool {
 public:
  thread_pool() : done(false), joiner(threads) {
    const unsigned thread_count = std::thread::hardware_concurrency();
    try {
      for (int i = 0; i < thread_count; i++) {
        threads.push_back(std::thread(&thread_pool::worker_thread, this));
      }
    } catch (std::system_error) {
      done = true;
      throw;
    }
  }

  ~thread_pool() { done = true; }

  template <typename FunctionType>
  void submit(FunctionType f) {
    work_queue.push(std::function<void()>(f));
  }

 private:
  void worker_thread() {
    while (!done) {
      std::function<void()> task;
      if (work_queue.try_pop(task)) {
        task();
      } else {
        std::this_thread::yield();
      }
    }
  }

  std::atomic<bool> done;
  threadsafe_queue<std::function<void()>> work_queue;
  std::vector<std::thread> threads;
  join_threads joiner;
};

#endif  // SIMPLE_THREAD_POOL_H_