#include <iostream>

template<typename T>
T max(T a, T b) {
  std::cout << "Custom max\n";
  return b < a ? a : b;
}
