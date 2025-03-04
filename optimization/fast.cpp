#include <benchmark/benchmark.h>
#include <algorithm>
#include <execution>
#include <numeric>

namespace bm = benchmark;

static void i32_addition(bm::State& state) {
  int32_t a, b, c;
  for (auto _ : state) {
    c = a + b;
  }
}

static void i32_addition_random(bm::State& state) {
  int32_t c = 0;
  for (auto _ : state) {
    c = std::rand() + std::rand();
  }
}

static void i32_addition_semi_random(bm::State& state) {
  int32_t a = std::rand(), b = std::rand(), c = 0;
  for (auto _ : state) {
    bm::DoNotOptimize(c = (++a) + (++b));
  }
}

BENCHMARK(i32_addition);
BENCHMARK(i32_addition_random);
BENCHMARK(i32_addition_random)->Threads(8);
BENCHMARK(i32_addition_semi_random);
BENCHMARK(i32_addition_semi_random)->Threads(8);

static void f64_sin(bm::State& state) {
  double argument = std::rand(), result = 0;
  for (auto _ : state) {
    bm::DoNotOptimize(result = std::sin(argument += 1.0));
  }
}

static void f64_sin_maclaurin(bm::State& state) {
  double argument = std::rand(), result = 0;
  for (auto _ : state) {
    argument += 1.0;
    result = argument - std::pow(argument, 3) / 6 + std::pow(argument, 5) / 120;
    bm::DoNotOptimize(result);
  }
}

static void f64_sin_maclaurin_powless(bm::State& state) {
  double argument = std::rand(), result = 0;
  for (auto _ : state) {
    argument += 1.0;
    result = (argument) - (argument * argument * argument) / 6.0 +
             (argument * argument * argument * argument * argument) / 120.0;
    bm::DoNotOptimize(result);
  }
}

BENCHMARK(f64_sin);
BENCHMARK(f64_sin_maclaurin);
BENCHMARK(f64_sin_maclaurin_powless);

static void i64_division(bm::State& state) {
  int64_t a = std::rand(), b = std::rand(), c = 0;
  for (auto _ : state) {
    bm::DoNotOptimize(c = (++a) / (++b));
  }
}

static void i64_division_by_const(bm::State& state) {
  int64_t money = 2147483647;
  int64_t a = std::rand(), c;
  for (auto _ : state) {
    bm::DoNotOptimize(c = (++a) / *std::launder(&money));
  }
}

static void i64_division_by_constexpr(bm::State& state) {
  constexpr int64_t b = 2147483647;
  int64_t a = std::rand(), c;
  for (auto _ : state) {
    bm::DoNotOptimize(c = (++a) / b);
  }
}

BENCHMARK(i64_division);
BENCHMARK(i64_division_by_const);
BENCHMARK(i64_division_by_constexpr);

constexpr size_t f32s_in_cache_line_k = 64 / sizeof(float);
constexpr size_t f32s_in_cache_line_half_k = f32s_in_cache_line_k / 2;

struct alignas(64) f32_array_t {
  float raw[f32s_in_cache_line_k * 2];
};

static void f32_pairwise_accumulation(bm::State& state) {
  f32_array_t a, b, c;
  for (auto _ : state) {
    for (size_t i = f32s_in_cache_line_half_k;
         i != f32s_in_cache_line_half_k * 3; ++i) {
      bm::DoNotOptimize(c.raw[i] = a.raw[i] + b.raw[i]);
    }
  }
}

static void f32_pairwise_accumulation_aligned(bm::State& state) {
  f32_array_t a, b, c;
  for (auto _ : state) {
    for (size_t i = 0; i != f32s_in_cache_line_half_k; ++i) {
      bm::DoNotOptimize(c.raw[i] = a.raw[i] + b.raw[i]);
    }
  }
}

BENCHMARK(f32_pairwise_accumulation);
BENCHMARK(f32_pairwise_accumulation_aligned);

static void sorting(bm::State& state) {
  auto count = static_cast<size_t>(state.range(0));
  auto include_preprocessing = static_cast<bool>(state.range(1));

  std::vector<int32_t> array(count);
  std::iota(array.begin(), array.end(), 1);

  for (auto _ : state) {
    if (!include_preprocessing) {
      state.PauseTiming();
    }

    // Reverse order is the most classical worst case,
    // but not the only one.
    std::reverse(array.begin(), array.end());
    if (!include_preprocessing) {
      state.ResumeTiming();
    }

    std::sort(array.begin(), array.end());
    bm::DoNotOptimize(array.size());
  }
}

static void upper_cost_of_pausing(bm::State& state) {
  int32_t a = std::rand(), c = 0;
  for (auto _ : state) {
    state.PauseTiming();
    ++a;
    state.ResumeTiming();
    bm::DoNotOptimize(c += a);
  }
}

BENCHMARK(sorting)->Args({3, false})->Args({3, true});
BENCHMARK(sorting)->Args({4, false})->Args({4, true});
BENCHMARK(upper_cost_of_pausing);

// template<typename execution_policy_t>
// static void super_sort(bm::State& state, execution_policy_t&& policy) {
//   auto count = static_cast<size_t>(state.range(0));
//   std::vector<int32_t> array(count);
//   std::iota(array.begin(), array.end(), 1);

//   for (auto _ : state) {
//     std::reverse(policy, array.begin(), array.end());
//     std::sort(policy, array.begin(), array.end());
//     bm::DoNotOptimize(array.size());
//   }

//   state.SetComplexityN(count);
//   state.SetItemsProcessed(count * state.iterations());
//   state.SetBytesProcessed(count * state.iterations() * sizeof(int32_t));
// }

// BENCHMARK_CAPTURE(super_sort, seq, std::execution::seq)
//     ->RangeMultiplier(8)
//     ->Range(1l << 20, 1l << 32)
//     ->MinTime(10)
//     ->Complexity(bm::oNLogN);

// BENCHMARK_CAPTURE(super_sort, par_unseq, std::execution::par_unseq)
//     ->RangeMultiplier(8)
//     ->Range(1l << 20, 1l << 32)
//     ->MinTime(10)
//     ->Complexity(bm::oNLogN);

BENCHMARK_MAIN();
