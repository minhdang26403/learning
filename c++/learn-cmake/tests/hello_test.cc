#include <blah.h>
#include <gtest/gtest.h>
#include <hello.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);

  PrintBlah();
  PrintHello();
}