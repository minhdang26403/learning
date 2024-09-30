#include <array>
#include <cassert>

template<typename T, std::size_t Maxsize>
class Stack {
 private:
  std::array<T, Maxsize> elems;
  std::size_t numElems;
 public:
  Stack();
  void push(T const& elem);
  void pop();
  T const& top() const;
  bool empty() const {
    return numElems == 0;
  }
  std::size_t size() const {
    return numElems;
  }
};
