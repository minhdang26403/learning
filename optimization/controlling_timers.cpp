#include <benchmark/benchmark.h>
#include <set>

static std::set<int> ConstructRandomSet(int64_t size) {
  std::set<int> data;

  return data;
}

static int RandomNumber() {
  return rand();
}

// Sometimes, it is necessary to do some work inside of that loop, every
// iteration, but without counting that time to the benchmark time. That is
// possible, although it is not recommended, since it has high overhead.
static void BM_SetInsert_With_Timer_Control(benchmark::State& state) {
  std::set<int> data;
  for (auto _ : state) {
    state.PauseTiming();  // Stop timers. They will not count until they are
                          // resumed.
    data = ConstructRandomSet(
        state.range(0));   // Do something that should not be measured.
    state.ResumeTiming();  // And resume timers. They are now counting again.
    // The rest will be measured.
    for (int j = 0; j < state.range(1); j++) {
      data.insert(RandomNumber());
    }
  }
}

BENCHMARK(BM_SetInsert_With_Timer_Control)->Ranges({{1<<10, 8<<10}, {128, 512}});

BENCHMARK_MAIN();
