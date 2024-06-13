#include "future.h"
#include "promise.h"
#include <future>
#include <gtest/gtest.h>
#include <thread>
#include "shared_future.h"

using namespace std::chrono_literals;

TEST(FutureTest, ConstructorTest) {
  // Default Constructor
  Future<int> f1;
  EXPECT_TRUE(!f1.valid());

  // a future is valid if it refers to a shared state
  Promise<int> p2;
  Future<int> f2 = p2.get_future();
  EXPECT_TRUE(f2.valid());

  // Move Constructor
  Future<int> f3{std::move(f2)};
  EXPECT_TRUE(!f2.valid());
  EXPECT_TRUE(f3.valid());
}

TEST(FutureTest, AssignmentTest) {
  Promise<int> p1;
  Future<int> f1 = p1.get_future();
  EXPECT_TRUE(f1.valid());

  Future<int> f2;
  EXPECT_TRUE(!f2.valid());
  // assignment from a valid future
  f2 = std::move(f1);
  EXPECT_TRUE(f2.valid());
  EXPECT_TRUE(!f1.valid());

  // assignment from an invalid future
  f2 = std::move(f1);
  EXPECT_TRUE(!f2.valid());
}

TEST(FutureTest, ShareTest) {
  Future<int> f1;
  auto sf1 = f1.share();
  EXPECT_TRUE(!f1.valid());
  EXPECT_TRUE(!f1.valid());

  Promise<int> p;
  Future<int> f2 = p.get_future();
  auto sf2 = f2.share();
  EXPECT_TRUE(!f2.valid());
  EXPECT_TRUE(sf2.valid());
}

TEST(FutureTest, WaitTest) {
  Promise<int> p;
  Future<int> f = p.get_future();
  std::thread t([p = std::move(p)]() mutable {
    std::this_thread::sleep_for(50ms);
    p.set_value(26);
  });
  t.detach();

  f.wait();

  EXPECT_EQ(f.get(), 26);
}

TEST(FutureTest, WaitForTest) {
  // Timeout
  Promise<int> p1;
  Future<int> f1 = p1.get_future();
  std::thread t1([p = std::move(p1)]() mutable {
    std::this_thread::sleep_for(20ms);
    p.set_value(26);
  });
  t1.detach();

  auto status1 = f1.wait_for(10ms);
  EXPECT_EQ(status1, std::future_status::timeout);

  // No timeout
  Promise<int> p2;
  Future<int> f2 = p2.get_future();
  std::thread t2([p = std::move(p2)]() mutable {
    std::this_thread::sleep_for(10ms);
    p.set_value(26);
  });
  t2.detach();

  auto status2 = f2.wait_for(20ms);
  EXPECT_EQ(status2, std::future_status::ready);

  // TODO: add `future_status::deferred` test here
}
