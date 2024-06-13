#include <iostream>
#include <thread>
#include <utility>

void f() { std::cout << "Call f()\n"; }

void g() { std::cout << "Call g()\n"; }

int main() {
  std::thread t1(f);
  // after this, t1 no longer has an associated thread of execution
  std::thread t2 = std::move(t1);

  t1 = std::thread(g);
  std::thread t3;
  t3 = std::move(t2);

  // need to detach or join t1 before assigning it a new thread
  t1.detach();
  t1 = std::move(t3);
  
  // Wait for thread to finish. Otherwise, t1 outlives `main` thread
  t1.join();
  return 0;
}
