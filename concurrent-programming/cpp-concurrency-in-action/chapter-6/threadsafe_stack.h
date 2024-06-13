#ifndef THREADSAFE_STACK_H_
#define THREADSAFE_STACK_H_

#include <exception>
#include <mutex>
#include <stack>

struct empty_stack : std::exception {
  const char* what() const throw() { return "empty stack"; }
};

template <typename T>
class threadsafe_stack {
 public:
  threadsafe_stack() = default;

  threadsafe_stack(const threadsafe_stack& other) {
    std::lock_guard<std::mutex> lk(other.m);
    data = other.data;
  }

  threadsafe_stack& operator=(const threadsafe_stack& other) = delete;

  void push(T new_value) {
    std::lock_guard<std::mutex> lk(m);
    data.push(std::move(new_value));
  }

  std::shared_ptr<T> pop() {
    std::lock_guard<std::mutex> lk(m);
    if (data.empty()) {
      throw empty_stack();
    }
    auto value_ptr = std::make_shared<T>(std::move(data.top()));
    data.pop();
    return value_ptr;
  }

  void pop(T& value) {
    std::lock_guard<std::mutex> lk(m);
    if (data.empty()) {
      throw empty_stack();
    }
    value = std::move(data.top());
    data.pop();
  }

  bool empty() const {
    std::lock_guard<std::mutex> lk(m);
    return data.empty();
  }

 private:
  std::stack<T> data;
  mutable std::mutex m;
};

#endif  // THREADSAFE_STACK_H_
