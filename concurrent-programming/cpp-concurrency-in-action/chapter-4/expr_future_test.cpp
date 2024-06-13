#include <gtest/gtest.h>
#include <type_traits>
#include <utility>

int bar() {
  return 42;
}

struct bazz {
  bazz() = delete;
  bazz(int x) {
    std::cout << x << '\n';
  };
  void operator()() {

  }
};

template<typename Func>
decltype(std::declval<Func>()()) foo(Func&& f) {
  // EXPECT_TRUE((std::is_same_v<decltype(f), int (*&&)()>));
  // EXPECT_TRUE((std::is_same_v<Func, int (*)()>));
  EXPECT_TRUE((std::is_same_v<Func, bazz&>));
  EXPECT_TRUE((std::is_same_v<decltype(f), bazz&>));
  return f();
}

TEST(ExperimentalFutureTest, BasicTest) {
  // auto result = foo(&bar);
  // result = result + 1;
  foo(bazz{1});
  // EXPECT_TRUE((std::is_same_v<decltype(result), int>));
}


