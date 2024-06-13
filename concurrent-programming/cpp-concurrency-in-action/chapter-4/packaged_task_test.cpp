#include <future>
#include <gtest/gtest.h>

#include "packaged_task.h"
#include "test_utils.h"

int fib(int n) {
  if (n < 3)
    return 1;
  else
    return fib(n - 1) + fib(n - 2);
}

TEST(PackagedTaskTest, ConstructorTest) {
  PackagedTask<int(double, float)> null_task;
  // no task and no shared state
  EXPECT_TRUE(!null_task.valid());

  // has shared state and task
  PackagedTask<int(int)> fib_task(&fib);

  PackagedTask<int(int)> new_task{std::move(fib_task)};
  // new task is valid
  EXPECT_TRUE(new_task.valid());
  EXPECT_NO_THROW(new_task.get_future());
  int x = 10;
  EXPECT_NO_THROW(new_task(x));
  // fib_task is no longer valid
  EXPECT_TRUE(!fib_task.valid());
  EXPECT_THROW_FUTURE_ERROR(fib_task.get_future(), std::future_errc::no_state);
  EXPECT_THROW_FUTURE_ERROR(fib_task(10), std::future_errc::no_state);
}

TEST(PackagedTaskTest, AssignmentTest) {
  PackagedTask<int(int)> null_task;
  PackagedTask<int(int)> fib_task(fib);
  
  null_task = std::move(fib_task);
  // null_task is valid
  EXPECT_TRUE(null_task.valid());
  EXPECT_NO_THROW(null_task.get_future());
  int x = 10;
  EXPECT_NO_THROW(null_task(x));

  // fib_task is no longer valid
  EXPECT_TRUE(!fib_task.valid());
  EXPECT_THROW_FUTURE_ERROR(fib_task.get_future(), std::future_errc::no_state);
  EXPECT_THROW_FUTURE_ERROR(fib_task(10), std::future_errc::no_state);
}

TEST(PackagedTaskTest, GetFutureTest) {
  PackagedTask<double(int, float)> null_task;
  EXPECT_THROW_FUTURE_ERROR(null_task.get_future(), std::future_errc::no_state);
  
  std::packaged_task<int(int)> std_fib_task(&fib);
  auto std_f = std_fib_task.get_future();
  PackagedTask<int(int)> fib_task(&fib);
  auto f = fib_task.get_future();
  
  EXPECT_TRUE(std_fib_task.valid());
  EXPECT_TRUE(std_fib_task.valid() == fib_task.valid());
  EXPECT_THROW_FUTURE_ERROR(fib_task.get_future(),
                            std::future_errc::future_already_retrieved);
}

TEST(PackagedTaskTest, InvokeOperatorTest) {
  PackagedTask<int(int)> fib_task(&fib);
  auto f = fib_task.get_future();
  int x = 10;
  fib_task(x);
  EXPECT_EQ(f.get(), 55);
  EXPECT_TRUE(fib_task.valid());
  EXPECT_THROW_FUTURE_ERROR(fib_task(10),
                            std::future_errc::promise_already_satisfied);
}

TEST(PackagedTest, ResetTest) {
  PackagedTask<int(int, int)> task([](int a, int b) { return std::pow(a, b); });
  auto result = task.get_future();
  task(2, 9);
  EXPECT_EQ(result.get(), 512);

  task.reset();
  EXPECT_TRUE(task.valid());
  EXPECT_NO_THROW(result = task.get_future());
  std::thread task_td(std::move(task), 2, 10);
  task_td.join();
  EXPECT_EQ(result.get(), 1024);
}
