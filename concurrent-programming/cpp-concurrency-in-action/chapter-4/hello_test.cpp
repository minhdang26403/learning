#include <gtest/gtest.h>
// #include <experimental/algorithm>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);

  std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

  // Use parallel version of std::sort
  // Problem getting experimental library to work
  // std::experimental::parallel::sort(std::execution::par, v.begin(),
  //                                   v.end());
}
