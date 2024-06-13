#include <iostream>
#include <thread>

class background_task {
 public:
  void operator()() const {
    std::cout << "Hello\n";
  }
};

int main() {
  // named object: ok
  background_task f;
  std::thread my_thread(f);
  my_thread.join();
  
  // most vexing parse problem
  // std::thread my_thread_error(background_task());
}