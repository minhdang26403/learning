#include <utility>
#include <iostream>

using namespace std;

class X {
 int i{};
};

void g(X&) {
  cout << "g() for variable\n";
}

void g(X const&) {
  cout << "g() for constant\n";
}

void g(X&&) {
  cout << "g() for movable object\n";
}

template<typename T>
void f(T&& val) {
  g(std::forward<T>(val));
}

int main() {
  X v;
  X const c;

  f(v);
  f(c);
  f(X());
  f(std::move(v));

  return 0;
}
