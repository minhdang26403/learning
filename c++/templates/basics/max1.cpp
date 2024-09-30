#include "max1.hpp"
#include <iostream>
#include <string>
#include <complex>

template<typename T>
void foo(T t) {
  // undeclared();
  undeclared(t);
  // static_assert(sizeof(int) > 10, "int too small");
  static_assert(sizeof(T) > 10, "T too small");
}

template<typename T = std::string>
void f(T = "") {

}

int main() {
  int i = 42;
  std::cout << "max(7,i): " << ::max(7, i) << '\n';

  double f1 = 3.4;
  double f2 = -6.7;
  std::cout << "max(f1,f2): " << ::max(f1, f2) << '\n';

  std::string s1 = "mathematics";
  std::string s2 = "math";
  std::cout << "max(s1,s2): " << ::max(s1, s2) << '\n';

  // std::complex<float> c1, c2;
  // ::max(c1, c2);
  // foo(s2);
  f();

  return 0;
}