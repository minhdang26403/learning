#include <benchmark/benchmark.h>

/*================================================================================*/

// DoNotOptimize(<expr>) forces the result of <expr> to be stored in either
// memory or a register. For GNU based compilers it acts as read/write barrier
// for global memory. More specifically it forces the compiler to flush pending
// writes to memory and reload any other values as necessary.
static void BM_test(benchmark::State& state) {
  for (auto _ : state) {
    int x = 0;
    for (int i = 0; i < 64000; i++) {
      // benchmark::DoNotOptimize(x += i);
      // x += i;
    }
  }
}

// Note that DoNotOptimize(<expr>) does not prevent optimizations on <expr> in
// any way. <expr> may even be removed entirely when the result is already
// known. For example:

// Example 1: `<expr>` is removed entirely.
//  int foo(int x) { return x + 42; }
//  while (...) DoNotOptimize(foo(0)); // Optimized to DoNotOptimize(42);

// Example 2: Result of '<expr>' is only reused.
//  int bar(int) __attribute__((const));
//  while (...) DoNotOptimize(bar(0)); // Optimized to:
// int __result__ = bar(0);
// while (...) DoNotOptimize(__result__);

BENCHMARK(BM_test);

/*================================================================================*/

// The second tool for preventing optimizations is ClobberMemory(). In essence
// ClobberMemory() forces the compiler to perform all pending writes to global
// memory. Memory managed by block scope objects must be "escaped" using
// DoNotOptimize(...) before it can be clobbered. In the below example
// ClobberMemory() prevents the call to v.push_back(42) from being optimized
// away.

static void BM_vector_push_back(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<int> v;
    v.reserve(1);
    auto data = v.data();  // Allow v.data() to be clobbered. Pass as non-const
    benchmark::DoNotOptimize(
        data);  // lvalue to avoid undesired compiler optimizations
    v.push_back(42);
    benchmark::ClobberMemory();  // Force 42 to be written to memory.
  }
}

BENCHMARK(BM_vector_push_back);

/*================================================================================*/

BENCHMARK_MAIN();