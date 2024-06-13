#include <iostream>
#include <thread>

// Waiting for threads to finish in exceptional circumstances

class thread_guard {
 public:
  explicit thread_guard(std::thread& t_) : t(t_) {}

  ~thread_guard() {
    if (t.joinable()) {
      t.join();
    }
  }
  thread_guard(const thread_guard&) = delete;
  thread_guard& operator=(const thread_guard&) = delete;
 private:
  std::thread& t;
};

void func() {
  std::cout << "Call func()\n";
}

void do_something() {

}

int main() {
  std::thread t(func);
  thread_guard g(t);

  // error. can bind to a named object
  // thread_guard z{std::thread(func)};

  // May throw exception. Use thread_guard for exception safety
  do_something();
}