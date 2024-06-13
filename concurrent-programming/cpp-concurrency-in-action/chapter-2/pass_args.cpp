#include <thread>
#include <iostream>

void inc(int& i) {
  i++;
}

class X {
 public:
  void print() {
    std::cout << "Hello from class X\n";
  }
};

void f(std::unique_ptr<X> uptr) {
  if (uptr == nullptr) {
    return;
  }
  std::cout << "Get unique_ptr\n";
}

int main() {
  int i = 0;
  // compile error if not use std::ref
  std::thread t(inc, std::ref(i));
  t.join();
  std::cout << "Value of i: " << i << '\n';

  // Call member function: need to pass `*this` object as first argument
  X x;
  std::thread t2(&X::print, &x);
  t2.join();

  // Must use std::move to transfer ownership
  // When the source object is temporary, the move is automatic
  // But with a named object, must use std::move explicitly
  auto uptr_x = std::make_unique<X>();
  std::thread t3(f, std::move(uptr_x));
  t3.join();

  return 0;
}