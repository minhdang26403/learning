#include <cassert>
#include <iostream>

#include <algorithm>
#include <execution>
#include "fp_quicksort.h"
#include <chrono>
#include <gtest/gtest.h>

std::ostream &operator<<(std::ostream &ostr, const std::list<int> &list) {
  for (auto &i : list)
    ostr << ' ' << i;

  return ostr;
}

TEST(FPQuickSortTest, SequentialSortTest) {
  std::list<int> input = {5, 7, 3, 4, 1, 9, 2, 8, 10, 6};
  auto result = sequential_quick_sort(input);
  std::list<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  EXPECT_TRUE(result == expected);
}

TEST(FPQuickSortTest, ParallelSortTest) {
  std::list<int> input = {5, 7, 3, 4, 1, 9, 2, 8, 10, 6};
  auto result = parallel_quick_sort(input);
  std::list<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  EXPECT_TRUE(result == expected);
}

TEST(FPQuickSortTest, BenchmarkQuickSort) {
  std::list<int> input;
  constexpr int N = 20000;

  for (int i = 0; i < N; i++) {
    input.push_back(rand() % N);
  }

  // Benchmark sequential quick sort running time
  auto clock_start = std::chrono::system_clock::now();
  auto seq_result = sequential_quick_sort(input);
  auto clock_end = std::chrono::system_clock::now();
  auto seq_time = std::chrono::duration_cast<std::chrono::milliseconds>(
      clock_end - clock_start);

  // Benchmark parallel quick sort running time
  clock_start = std::chrono::system_clock::now();
  auto par_result = parallel_quick_sort(input);
  clock_end = std::chrono::system_clock::now();
  auto par_time = std::chrono::duration_cast<std::chrono::milliseconds>(
      clock_end - clock_start);

  EXPECT_TRUE(seq_result == par_result);

  std::cout << "Sequential quicksort: " << seq_time.count() << '\n'
            << "Parallel quicksort: " << par_time.count() << '\n';
}
