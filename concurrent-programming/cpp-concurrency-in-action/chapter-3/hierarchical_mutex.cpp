#include <iostream>
#include <mutex>

class hierarchical_mutex {
 public:
  explicit hierarchical_mutex(unsigned long value)
      : hierarchy_value(value), previous_hierarchy_value(0) {}

  void lock() {
    check_for_hierarchy_violation();
    internal_mutex.lock();
    update_hierarchy_value();
  }

  void unlock() {
    this_thread_hierarchy_value = previous_hierarchy_value;
    internal_mutex.unlock();
  }

  bool try_lock() {
    check_for_hierarchy_violation();
    if (!internal_mutex.try_lock()) {
      return false;
    }
    update_hierarchy_value();
    return true;
  }
  static thread_local unsigned long this_thread_hierarchy_value;

 private:
  void check_for_hierarchy_violation() {
    if (this_thread_hierarchy_value <= hierarchy_value) {
      throw std::logic_error("mutex hierarchy violated");
    }
  }

  void update_hierarchy_value() {
    previous_hierarchy_value = this_thread_hierarchy_value;
    this_thread_hierarchy_value = hierarchy_value;
  }

  std::mutex internal_mutex;
  const unsigned long hierarchy_value;
  unsigned long previous_hierarchy_value;
  // static thread_local unsigned long this_thread_hierarchy_value;
};

// need to provide definition
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value =
    ULONG_MAX;

int main() {
  hierarchical_mutex high_level_mutex(10);
  hierarchical_mutex low_level_mutex(5);

  high_level_mutex.lock();
  std::cout << hierarchical_mutex::this_thread_hierarchy_value << '\n';

  low_level_mutex.lock();
  std::cout << hierarchical_mutex::this_thread_hierarchy_value << '\n';

  low_level_mutex.unlock();
  std::cout << hierarchical_mutex::this_thread_hierarchy_value << '\n';

  high_level_mutex.unlock();
  std::cout << hierarchical_mutex::this_thread_hierarchy_value << '\n';

  // Error if reverse the order of `unlock` on `low_level_mutex` and
  // `high_level_mutex`

  // When unlocking in reverse order of locking, the
  // `this_thread_hierarchy_value` will be 10, not ULONG_MAX
  
  // hierarchical_mutex another_mutex(12);
  // another_mutex.lock();
  // another_mutex.unlock();

  return 0;
}