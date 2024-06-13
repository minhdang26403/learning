#include <atomic>
#include <iostream>

class spinlock_mutex {
 public:
  spinlock_mutex() = default;

  void lock() {
    while (flag.test_and_set(std::memory_order_acquire)) {}
  }

  void unlock() {
    flag.clear(std::memory_order_release);
  }

 private:
  std::atomic_flag flag;
};


int main() {
  std::atomic_flag f;
  std::cout << f.test() << '\n';

  return 0;
}
