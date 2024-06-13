#include "future.h"
#include "promise.h"
#include "test_utils.h"
#include <exception>
#include <future>
#include <gtest/gtest.h>
#include <numeric>
#include <stdexcept>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

TEST(PromiseTest, NoStateTest) {
  Promise<int> p1;
  auto p2 = std::move(p1);

  EXPECT_THROW_FUTURE_ERROR(p1.get_future(), std::future_errc::no_state);
  EXPECT_THROW_FUTURE_ERROR(p1.set_value(1), std::future_errc::no_state);

  Promise<int> p3;
  p3 = std::move(p2);
  EXPECT_THROW_FUTURE_ERROR(p2.get_future(), std::future_errc::no_state);
  EXPECT_THROW_FUTURE_ERROR(p2.set_value(1), std::future_errc::no_state);
}

TEST(PromiseTest, GetFutureTest) {
  Promise<int> p;
  EXPECT_NO_THROW(p.get_future());
  EXPECT_THROW_FUTURE_ERROR(p.get_future(),
                            std::future_errc::future_already_retrieved);
}

TEST(PromiseTest, SetValueTest) {
  Promise<int> p;
  auto f = p.get_future();
  std::thread t([p = std::move(p)]() mutable { p.set_value(1); });
  t.detach();

  // promise is invalid after being moved
  EXPECT_THROW_FUTURE_ERROR(p.get_future(), std::future_errc::no_state);
  EXPECT_EQ(f.get(), 1);

  Promise<int> p2;
  EXPECT_NO_THROW(p2.set_value(1));
  EXPECT_THROW_FUTURE_ERROR(p2.set_value(2),
                            std::future_errc::promise_already_satisfied);
}

TEST(PromiseTest, SetExceptionTest) {
  Promise<int> p;
  auto f = p.get_future();
  std::thread t([p = std::move(p)]() mutable {
    try {
      // code that may throw
      throw std::runtime_error(std::string{"Example"});
    } catch (...) {
      try {
        p.set_exception(std::current_exception());
      } catch (...) {
        // set_exception() may throw too
        ;
      }
    }
  });
  t.detach();

  try {
    f.get();
  } catch (const std::exception &e) {
    EXPECT_EQ(e.what(), std::string{"Example"});
  }
}

TEST(PromiseTest, BrokenPromiseTest) {
  Promise<int> p;
  auto f = p.get_future();
  std::thread t([p = std::move(p)]() mutable {
    // Do nothing
    ;
    // `Promise` object is destroyed here
  });
  t.detach();

  // The promise is destroyed before this thread can access it
  std::this_thread::sleep_for(5ms);
  EXPECT_THROW_FUTURE_ERROR(f.get(), std::future_errc::broken_promise);
}

TEST(PromiseTest, GeneralTest) {
  // Demonstrate using Promise<int> to transmit a result between threads.
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
  Promise<int> accumulate_promise;
  auto accumulate_future = accumulate_promise.get_future();
  std::thread work_thread(
      [](auto first, auto last, Promise<int> accumulate_promise) {
        int sum = std::accumulate(first, last, 0);
        accumulate_promise.set_value(sum);
      },
      numbers.begin(), numbers.end(), std::move(accumulate_promise));
  EXPECT_EQ(accumulate_future.get(), 21);
  work_thread.join();

  // Demonstrate using Promise to signal state between threads
  Promise<std::string> barrier;
  auto barrier_future = barrier.get_future();
  std::thread new_work_thread(
      [](Promise<std::string> barrier) {
        std::this_thread::sleep_for(50ms);
        barrier.set_value("ok");
      },
      std::move(barrier));
  barrier_future.wait();
  new_work_thread.join();
}
