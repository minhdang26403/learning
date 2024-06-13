#include <gtest/gtest.h>
#include "async.h"

#include <numeric>

struct X {
  int operator()(int i) {
    return i + 10;
  }
};

template <typename RandomIt> int parallel_sum(RandomIt beg, RandomIt end) {
  auto len = end - beg;
  if (len < 1000)
    return std::accumulate(beg, end, 0);

  RandomIt mid = beg + len / 2;
  auto handle =
      Async(parallel_sum<RandomIt>, mid, end);
  int sum = parallel_sum(beg, mid);
  return sum + handle.get();
}

int f(int x) {
  return x + 10;
}

TEST(AsyncTest, BasicTest) {
  std::vector<int> v(10000, 1);
  EXPECT_EQ(parallel_sum(v.begin(), v.end()), 10000);
  auto future = Async(f, 43);
  EXPECT_EQ(future.get(), 53);
  
  // TODO: why doesn't work with function object???
  // X x;
  // auto f = Async(x, 43);
  // EXPECT_EQ(f.get(), 53);
}
