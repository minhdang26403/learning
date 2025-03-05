#include <atomic>
#include <thread>

std::atomic<bool> flag0(false), flag1(false);
std::atomic<int> turn(0);

void p0() {
  flag0.store(true, std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_seq_cst);

  while (flag1.load(std::memory_order_relaxed)) {
    if (turn.load(std::memory_order_relaxed) != 0) {
      flag0.store(false, std::memory_order_relaxed);
      while (turn.load(std::memory_order_relaxed) != 0) {
      }
      flag0.store(true, std::memory_order_relaxed);
      std::atomic_thread_fence(std::memory_order_seq_cst);
    }
  }
  std::atomic_thread_fence(std::memory_order_acquire);

  // critical section

  turn.store(1, std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_release);
  flag0.store(false, std::memory_order_relaxed);
}

void p1() {
  flag1.store(true, std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_seq_cst);

  while (flag0.load(std::memory_order_relaxed)) {
    if (turn.load(std::memory_order_relaxed) != 1) {
      flag1.store(false, std::memory_order_relaxed);
      while (turn.load(std::memory_order_relaxed) != 1) {
      }
      flag1.store(true, std::memory_order_relaxed);
      std::atomic_thread_fence(std::memory_order_seq_cst);
    }
  }
  std::atomic_thread_fence(std::memory_order_acquire);

  // critical section

  turn.store(0, std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_release);
  flag1.store(false, std::memory_order_relaxed);
}

int main() {
  std::thread t1(p0);
  std::thread t2(p1);

  t1.join();
  t2.join();

  return 0;
}
