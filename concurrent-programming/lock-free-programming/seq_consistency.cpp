// #include <atomic>
#include <iostream>
#include <thread>

/**
 * @brief A simple program to show that the CPU (ARM) does not provide
 * sequential consistency.
 */

int x, y, tmp1, tmp2;

void thread1() {
  x = 1;
  // std::atomic_thread_fence(std::memory_order_seq_cst);
  tmp1 = y;
}

void thread2() {
  y = 1;
  // std::atomic_thread_fence(std::memory_order_seq_cst);
  tmp2 = x;
}

int main() {
  while (1) {
    x = y = tmp1 = tmp2 = 0;
    std::thread t1(thread1);
    std::thread t2(thread2);

    t1.join();
    t2.join();
    // When reorderings happens:
    // + load of `tmp1` is reordered before store to `x`
    // + load of `tmp2` is reordered before store to `y`
    // We get tmp1 = tmp2 = 0.
    // To prevent this reordering, we use an memory barrier that enforces
    // sequential consistency.
    if (tmp1 == 0 && tmp2 == 0) {
      std::cout << "Got memory reordering\n";
      break;
    }
  }

  return 0;
}
