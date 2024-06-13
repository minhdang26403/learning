#ifndef THREADSAFE_QUEUE_H_
#define THREADSAFE_QUEUE_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template <typename T>
class threadsafe_queue {
 public:
  threadsafe_queue() = default;

  threadsafe_queue(const threadsafe_queue& other) {
    std::lock_guard<std::mutex> lk(other.mut);
    data_queue = other.data_queue;
  }

  threadsafe_queue& operator=(const threadsafe_queue&) = delete;

  void push(T new_value) {
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(std::move(new_value));
    data_cond.notify_one();
  }

  void wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    value = std::move(data_queue.front());
    data_queue.pop();
  }

  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    auto value_ptr = std::make_shared<T>(std::move(data_queue.front()));
    data_queue.pop();
    return value_ptr;
  }

  bool try_pop(T& value) {
    std::unique_lock<std::mutex> lk(mut);
    if (data_queue.empty()) {
      return false;
    }
    value = std::move(data_queue.front());
    data_queue.pop();
    return true;
  }

  std::shared_ptr<T> try_pop() {
    std::unique_lock<std::mutex> lk(mut);
    if (data_queue.empty()) {
      return std::shared_ptr<T>();
    }
    auto value_ptr = std::make_shared<T>(std::move(data_queue.front()));
    data_queue.pop();
    return value_ptr;
  }

  bool empty() const { return data_queue.empty(); }

 private:
  mutable std::mutex mut;
  std::queue<T> data_queue;
  std::condition_variable data_cond;
};

#endif  // THREADSAFE_QUEUE_H_