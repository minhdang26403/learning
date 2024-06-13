#include <iostream>
#include <numeric>

#include "waitable_thread_pool.h"

template <typename Iterator, typename T>
struct accumulate_block {
  T operator()(Iterator first, Iterator last) {
    return std::accumulate(first, last, T());
  }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
  size_t length = std::distance(first, last);
  if (length == 0) {
    return init;
  }

  // block should not be too small; otherwise, the over head of thread_pool and
  // future dominates
  int block_size = 25;
  int num_blocks = (length + block_size - 1) / block_size;

  std::vector<std::future<T>> futures(num_blocks - 1);
  thread_pool pool;

  Iterator block_start = first;
  for (int i = 0; i < num_blocks - 1; i++) {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    futures[i] =
        pool.submit(accumulate_block<Iterator, T>(), block_start, block_end);
    block_start = block_end;
  }

  T last_result = accumulate_block<Iterator, T>()(block_start, last);
  T result = init;
  for (auto& future : futures) {
    result += future.get();
  }
  result += last_result;

  return result;
}

int main() {
  std::vector<int> v(10, 1000);

  auto result = parallel_accumulate(v.begin(), v.end(), 0);
  std::cout << result << '\n';

  return 0;
}