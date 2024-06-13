#include <gtest/gtest.h>
#include "shared_future.h"
#include "promise.h"

using namespace std::chrono_literals;

TEST(SharedFutureTest, ConstructorTest) {
  SharedFuture<int> sf1;
  EXPECT_TRUE(!sf1.valid());
  SharedFuture<int> sf2{sf1};
  EXPECT_TRUE(!sf2.valid());

  Promise<int> p;
  Future<int> f = p.get_future();
  SharedFuture<int> sf3{std::move(f)};
  EXPECT_TRUE(sf3.valid());
  EXPECT_TRUE(!f.valid());

  SharedFuture<int> sf4 {sf3};
  EXPECT_TRUE(sf3.valid());
  EXPECT_TRUE(sf4.valid());

  SharedFuture<int> sf5{std::move(sf3)};
  EXPECT_TRUE(!sf3.valid());
  EXPECT_TRUE(sf4.valid());
  EXPECT_TRUE(sf5.valid());
}

TEST(SharedFutureTest, AssignmentTest) {
  SharedFuture<int> sf1;
  SharedFuture<int> sf2;
  sf2 = sf1;
  EXPECT_TRUE(sf1.valid() == sf2.valid());

  Promise<int> p;
  auto f = p.get_future();
  auto sf3 = f.share();
  sf2 = sf3;
  EXPECT_TRUE(sf2.valid() == sf3.valid());

  sf1 = std::move(sf2);
  EXPECT_TRUE(sf1.valid());
  EXPECT_TRUE(!sf2.valid());
}

TEST(FutureTest, WaitTest) {
  Promise<int> p;
  Future<int> f = p.get_future();
  auto sf = f.share();
  std::thread t([p = std::move(p)]() mutable {
    std::this_thread::sleep_for(50ms);
    p.set_value(26);
  });
  t.detach();

  sf.wait();

  EXPECT_EQ(sf.get(), 26);
}

TEST(FutureTest, WaitForTest) {
  // Timeout
  Promise<int> p1;
  Future<int> f1 = p1.get_future();
  auto sf1 = f1.share();
  std::thread t1([p = std::move(p1)]() mutable {
    std::this_thread::sleep_for(20ms);
    p.set_value(26);
  });
  t1.detach();

  auto status1 = sf1.wait_for(10ms);
  EXPECT_EQ(status1, std::future_status::timeout);

  // No timeout
  Promise<int> p2;
  Future<int> f2 = p2.get_future();
  auto sf2 = f2.share();
  std::thread t2([p = std::move(p2)]() mutable {
    std::this_thread::sleep_for(10ms);
    p.set_value(26);
  });
  t2.detach();

  auto status2 = sf2.wait_for(20ms);
  EXPECT_EQ(status2, std::future_status::ready);

  // TODO: add `future_status::deferred` test here
}
