#include <iostream>

// #include "simple_thread_pool.h"
// #include "waitable_thread_pool.h"
#include "local_thread_pool.h"
#include "function_wrapper.h"

void hello1() { std::cout << "Hello from 1\n"; }

void hello2() { std::cout << "Hello from 2\n"; }

void hello(int i) {
  std::cout << "Hello " << i << '\n';
}

int main() {
  thread_pool pool;
  pool.submit(hello1);
  pool.submit(hello2);

  function_wrapper wrapper(hello1);
  hello1();

  pool.submit(hello, 1);

  return 0;
}
