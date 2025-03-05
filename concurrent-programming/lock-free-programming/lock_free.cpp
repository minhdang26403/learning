#include <atomic>
#include <cassert>
#include <thread>

std::atomic<int> x{0}, y{0};

void thread1() {
  x.store(1, std::memory_order_relaxed);
  y.store(1, std::memory_order_relaxed);
}

void thread2() {
  int a = y.load(std::memory_order_relaxed);
  int b = x.load(std::memory_order_relaxed);
  if (a == 1) {
    assert(b == 1);
  }
}

int main() {
  std::thread t1(thread1);
  std::thread t2(thread2);

  t1.join();
  t2.join();

  return 0;
}
