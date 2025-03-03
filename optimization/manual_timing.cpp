#include <benchmark/benchmark.h>
#include <chrono>
#include <thread>

// For benchmarking something for which neither CPU time nor real-time are
// correct or accurate enough, completely manual timing is supported using the
// `UseManualTime` function.
static void BM_ManualTiming(benchmark::State& state) {
  int microseconds = state.range(0);
  std::chrono::duration<double, std::micro> sleep_duration{
      static_cast<double>(microseconds)};

  for (auto _ : state) {
    auto start = std::chrono::high_resolution_clock::now();
    // Simulate some useful workload with a sleep
    std::this_thread::sleep_for(sleep_duration);
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}

BENCHMARK(BM_ManualTiming)->Range(1, 1 << 17)->UseManualTime()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
