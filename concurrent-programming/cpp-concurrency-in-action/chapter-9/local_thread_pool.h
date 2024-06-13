#ifndef LOCAL_THREAD_POOL_H_
#define LOCAL_THREAD_POOL_H_

#include <atomic>
#include <future>
#include <type_traits>
#include <queue>
#include <iostream>

#include "../chapter-6/fine_grained_queue.h"
#include "function_wrapper.h"
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

  ~thread_pool() {
    // while (!pool_work_queue.empty()) {
    //   ;
    // }
    done = true;
  }

  // template< class F, class... ArgTypes >
  // class result_of<F(ArgTypes...)>;
  // removed in C++20

  // instead, use (since C++17):
  // template< class F, class... ArgTypes >
  // class invoke_result;

  template <typename F, typename... Args>
  std::future<std::invoke_result_t<F, Args...>> submit(F&& f, Args&&... args) {
    using result_type = std::invoke_result_t<F, Args...>;
    // no argument types in the template of `packaged_task` since we already
    // bind these arguments to the function
    std::packaged_task<result_type()> task(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<result_type> res(task.get_future());
    function_wrapper wrapper(std::move(task));
    if (local_work_queue) {
      local_work_queue->push(std::move(wrapper));
    } else {
      pool_work_queue.push(std::move(wrapper));
    }

    return res;
  }

  void run_pending_task() {
    function_wrapper task;
    if (local_work_queue && !local_work_queue->empty()) {
      task = std::move(local_work_queue->front());
      local_work_queue->pop();
      task();
    } else if (pool_work_queue.try_pop(task)) {
      task();
    } else {
      std::this_thread::yield();
    }
  }

 private:
  void worker_thread() {
    local_work_queue.reset(new local_queue_type());
    while (!done) {
      run_pending_task();
    }
  }

  std::atomic<bool> done;
  threadsafe_queue<function_wrapper> pool_work_queue;
  using local_queue_type = std::queue<function_wrapper>;
  // declaration (uses 'static')
  static thread_local std::unique_ptr<local_queue_type> local_work_queue;
  std::vector<std::thread> threads;
  join_threads joiner;
};

// definition (does not use 'static')
thread_local std::unique_ptr<thread_pool::local_queue_type> thread_pool::local_work_queue;

#endif  // LOCAL_THREAD_POOL_H_