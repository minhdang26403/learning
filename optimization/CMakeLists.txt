cmake_minimum_required(VERSION 3.14) # CMake 3.14+ supports FetchContent
project(LearnGoogleBenchmark)

# ==============================Compiler Flags==============================

# Enable C++20 (or later if needed)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For clangd.
set(CMAKE_CXX_STANDARD 20) # Compile as C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require C++20 support.

# Check if CMAKE_ENABLE_OPENMP is ON
if(CMAKE_ENABLE_OPENMP)
    # Enable OpenMP by adding the compiler flag -fopenmp
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    message(STATUS "OpenMP enabled")
endif()

# ==============================Third-party Libraries==============================

# FetchContent module to download and build Google Benchmark and GoogleTest
include(FetchContent)

# Download Google Test (required for Google Benchmark)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0 # Use the main branch or specify a specific commit/tag
)

# Download Google Benchmark
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.1 # Use the main branch or specify a specific commit/tag
)

# Fetch the content (download and build)
FetchContent_MakeAvailable(googletest googlebenchmark)

# ==============================Executables==============================
list(APPEND SOURCES

)

foreach(SOURCE IN LISTS SOURCES)
  add_executable(${SOURCE} ${SOURCE}.cpp)
endforeach()

# ==============================Tests==============================
enable_testing()

list(APPEND TESTS
)

foreach(TEST IN LISTS TESTS)
  add_executable(${TEST} ${TEST}.cpp)
  target_link_libraries(${TEST} GTest::gtest_main)
  gtest_discover_tests(${TEST})
endforeach()

# ==============================Benchmarks==============================

# Collect all executables (can be expanded easily)
list(APPEND BENCHMARKS
  bench
  controlling_timers
  custom_stat
  fast
  intro
  manual_timing
  prevent_opt
  timer
) # Add more source files here

# Loop over each source file to create an executable
foreach(BENCHMARK IN LISTS BENCHMARKS)
  add_executable(${BENCHMARK} ${BENCHMARK}.cpp)
  target_link_libraries(${BENCHMARK} benchmark::benchmark)
endforeach()
