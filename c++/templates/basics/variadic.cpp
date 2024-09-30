#include <iostream>
#include <type_traits>
#include <stack>

template<typename T1, typename... TN>
constexpr bool isHomogeneous(T1, TN...) {
  return (std::is_same_v<T1, TN>&&...);
}

template<typename T, std::size_t N>
struct array {

};

template<typename T, typename... U>
array(T, U...) -> array<std::enable_if_t<(std::is_same_v<T, U> && ...), T>, (1 + sizeof...(U))>;

template<typename T>
constexpr T pi{3.14159};

template<typename T>
void print(const std::vector<T> &v) {
  using iterator = typename std::vector<T>::const_iterator;
  for (iterator it = v.begin(); it != v.end(); it++) {
    std::cout << *it << ' ';
  }
}

int main() {
  std::cout << isHomogeneous("hello", " ", "world", "!");

  std::vector<int> v = {1, 2, 3, 4, 5};
  print(v);

  std::stack<int, std::vector<int>> s;


  return 0;
}