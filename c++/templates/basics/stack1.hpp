#include <vector>
#include <cassert>

template<typename T>
class Stack {
 public:
  Stack() = default;
  Stack(T const& elem) : elems({elem}) {}
  void push(T const& elem);
  void pop();
  T const& top() const;
  bool empty() const {
    return elems.empty();
  }

  void printOn(std::ostream& strm) const {
    for (T const &elem : elems) {
      strm << elem << ' ';
    }
  }

  friend std::ostream& operator<< (std::ostream& strm, Stack<T> const &s) {
    s.printOn(strm);
    return strm;
  }
 private:
  std::vector<T> elems;
};

Stack(char const*) -> Stack<std::string>;

template<typename T>
void Stack<T>::push(T const& elem) {
  elems.push_back(elem);
}

template<typename T>
void Stack<T>::pop() {
  assert(!elems.empty());
  elems.pop_back();
}

template<typename T>
T const& Stack<T>::top() const {
  assert(!elems.empty());
  return elems.back();
}
