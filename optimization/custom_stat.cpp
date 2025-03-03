#include <benchmark/benchmark.h>
#include <algorithm>

void BM_spin_empty(benchmark::State& state) {
  for (auto _ : state) {
    for (int x = 0; x < state.range(0); x++) {
      benchmark::DoNotOptimize(x);
    }
  }
}

BENCHMARK(BM_spin_empty)
    ->Repetitions(3)
    ->ComputeStatistics("max",
                        [](const std::vector<double>& v) -> double {
                          return *(
                              std::max_element(std::begin(v), std::end(v)));
                        })
    ->ComputeStatistics(
        "ratio",
        [](const std::vector<double>& v) -> double {
          return v.front() / v.back();
        },
        benchmark::StatisticUnit::kPercentage)
    ->Arg(512);

BENCHMARK_MAIN();