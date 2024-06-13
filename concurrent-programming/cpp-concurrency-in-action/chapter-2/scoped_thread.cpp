#include <iostream>
#include <thread>
#include <utility>

class scoped_thread {
 public:
  // if parameter is `std::thread`, call to constructor with rvalue argument is
  // ambiguous since can bind to this constructor or copy constructor (const
  // std::thread&)
  explicit scoped_thread(std::thread&& t_) : t(std::move(t_)) {
    if (!t.joinable()) {
      throw std::logic_error("No thread");
    }
  }

  ~scoped_thread() { t.join(); }

  scoped_thread(const std::thread&) = delete;
  scoped_thread& operator=(const std::thread&) = delete;

 private:
  std::thread t;
};

void f(int i) { std::cout << "Get value " << i << '\n'; }

int main() {
  int i = 10;

  scoped_thread t(std::thread(f, i));

  return 0;
}
