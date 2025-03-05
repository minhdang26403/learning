cmake_minimum_required(VERSION 3.14) # CMake 3.14+ supports FetchContent
project(LearnLockFreeProgramming)

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

# Check if the user has enabled assembly output
if(GENERATE_ASSEMBLY)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -S -fverbose-asm")
    message(STATUS "Generating assembly output")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror -Wstrict-aliasing=2 -Wno-unqualified-std-cast-call -Wno-sign-conversion")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fsanitize=${SANITIZER} -fno-omit-frame-pointer -fno-optimize-sibling-calls")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mtune=native -march=native")

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

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
  dekker
  lock_free
  memory_ordering
  seq_consistency
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
list(APPEND BENCHMARKS
  
)

foreach(BENCHMARK IN LISTS BENCHMARKS)
  add_executable(${BENCHMARK} ${BENCHMARK}.cpp)
  target_link_libraries(${BENCHMARK} benchmark::benchmark)
endforeach()
