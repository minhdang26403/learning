#include "fine_grained_queue.h"

#include <iostream>
#include <thread>

void add_elements(threadsafe_queue<int>& q) {
  for (int i = 0; i < 5; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // std::cout << "worker\n";
    q.push(i);
  }
}

int main() {
  threadsafe_queue<int> q;

  std::thread worker_thread(add_elements, std::ref(q));

  for (int i = 0; i < 5; i++) {
    // std::cout << "main\n";
    auto val = q.wait_and_pop();
    std::cout << *val << ' ';
  }
  std::cout << '\n';
  worker_thread.join();

  return 0;
}