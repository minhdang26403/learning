#include <benchmark/benchmark.h>
#include <iostream>

static void BM_memcpy(benchmark::State& state) {
  char* src = new char[state.range(0)];
  char* dst = new char[state.range(0)];
  memset(src, 'x', state.range(0));
  for (auto _ : state)
    memcpy(dst, src, state.range(0));
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
  delete[] dst;
}

// The arguments in the range are generated in multiples of eight by default
// [8, 64, 512, 4k, 8k]
// BENCHMARK(BM_memcpy)->Arg(8)->Arg(64)->Arg(512)->Arg(4 << 10)->Arg(8 << 10);

// Range multiplier is changed to multiples of two.
// [8, 16, 32, 64, 128, 256, 512, 1024, 2k, 4k, 8k]
BENCHMARK(BM_memcpy)->RangeMultiplier(2)->Range(8, 8 << 10);

static void BM_DenseRange(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<int> v(state.range(0), state.range(0));
    auto data = v.data();
    benchmark::DoNotOptimize(data);
    benchmark::ClobberMemory();
  }
}

BENCHMARK(BM_DenseRange)->DenseRange(0, 1024, 128);

template<class... Args>
void BM_takes_args(benchmark::State& state, Args&&... args) {
  auto args_tuple = std::make_tuple(std::move(args)...);
  for (auto _ : state) {
    std::cout << std::get<0>(args_tuple) << ": " << std::get<1>(args_tuple)
              << '\n';
  }
}

// Registers a benchmark named "BM_takes_args/int_string_test" that passes
// the specified values to `args`.
BENCHMARK_CAPTURE(BM_takes_args, int_string_test, 42, std::string("abc"));

// Registers the same benchmark "BM_takes_args/int_test" that passes
// the specified values to `args`.
BENCHMARK_CAPTURE(BM_takes_args, int_test, 42, 43);

static void BM_StringCompare(benchmark::State& state) {
  std::string s1(state.range(0), '-');
  std::string s2(state.range(0), '-');
  double numFoos = 0;
  for (auto _ : state) {
    auto comparison_result = s1.compare(s2);
    benchmark::DoNotOptimize(comparison_result);
    numFoos++;
  }

  // Sets a simple counter
  // state.counters["Foo"] = numFoos;

  // Sets the counter as a rate. It will be presented divided by the duration of
  // the benchmark.
  // Meaning: per one second, how many 'foo's are processed?
  state.counters["Foo"] =
      benchmark::Counter(numFoos, benchmark::Counter::kIsRate);

  // Sets the counter as a rate. It will be presented divided by the duration of
  // the benchmark, and the result inverted.
  // Meaning: how many seconds it takes to process one 'foo'?
  state.counters["FooInvRate"] = benchmark::Counter(
      numFoos, benchmark::Counter::kIsRate | benchmark::Counter::kInvert);

  // Sets the counter as a thread-average quantity. It will be presented divided
  // by the number of threads.
  state.counters["FooAvg"] =
      benchmark::Counter(numFoos, benchmark::Counter::kAvgThreads);

  // There's also a combined flag:
  state.counters["FooAvgRate"] =
      benchmark::Counter(numFoos, benchmark::Counter::kAvgThreadsRate);

  // This says that we process with the rate of state.range(0) bytes every
  // iteration:
  state.counters["BytesProcessed"] = benchmark::Counter(
      state.range(0), benchmark::Counter::kIsIterationInvariantRate,
      benchmark::Counter::OneK::kIs1024);

  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_StringCompare)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 18)
    ->Complexity(benchmark::oN);

// Asymptotic complexity might also be calculated automatically
// BENCHMARK(BM_StringCompare)
//     ->RangeMultiplier(2)
//     ->Range(1 << 10, 1 << 18)
//     ->Complexity();

BENCHMARK_MAIN();
