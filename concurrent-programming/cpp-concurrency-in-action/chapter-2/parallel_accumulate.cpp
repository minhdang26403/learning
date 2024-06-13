#include <iostream>
#include <numeric>
#include <thread>

template <typename Iterator, typename T>
struct accumulate_block {
  void operator()(Iterator first, Iterator last, T& result) {
    result = std::accumulate(first, last, result);
  }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
  size_t length = std::distance(first, last);
  if (length == 0) {
    return init;
  }

  int min_per_thread = 25;
  int max_threads = (length + min_per_thread - 1) / min_per_thread;

  int hardware_threads = std::thread::hardware_concurrency();
  int num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

  int block_size = length / num_threads;

  std::vector<T> results(num_threads);
  std::vector<std::thread> threads(num_threads - 1);

  Iterator block_start = first;
  for (int i = 0; i < num_threads - 1; i++) {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start,
                             block_end, std::ref(results[i]));
    block_start = block_end;
  }

  accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
  std::for_each(threads.begin(), threads.end(),
                std::mem_fn(&std::thread::join));

  return std::accumulate(results.begin(), results.end(), init);
}

int main() {
  std::vector<int> v(10, 1000);

  auto result = parallel_accumulate(v.begin(), v.end(), 0);
  std::cout << result << '\n';

  return 0;
}
