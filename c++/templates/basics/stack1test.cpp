#include "stack1.hpp"
#include <iostream>
#include <string>

template<typename T>
struct ValueWithComment {
  T value;
  std::string comment;
};

ValueWithComment(char const*, char const *) -> ValueWithComment<std::string>;

int main() {
  Stack stringStack{"bottom"};
  // stringStack.push("yejin"); error type of stringStack is char const[7]

  ValueWithComment<int> vc;
  vc.value = 42;
  vc.comment = "initial value";

  ValueWithComment vc2 = {"hello", "initial value"};
}