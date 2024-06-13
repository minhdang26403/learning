#include <iostream>
#include <thread>

using namespace std::chrono_literals;

void hello() {
  std::this_thread::sleep_for(10ms);
  std::cout << "Hello Concurrent World\n";
}

int main() {
  std::thread parent_thread;
  {
    // we will move this child_thread to outside
    std::thread child_thread(hello);
    parent_thread = std::move(child_thread);
  }
  parent_thread.join();
}
