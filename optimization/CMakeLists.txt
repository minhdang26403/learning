cmake_minimum_required(VERSION 3.14) # CMake 3.14+ supports FetchContent
project(LearnGoogleBenchmark)

# Enable C++20 (or later if needed)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For clangd.
set(CMAKE_CXX_STANDARD 20) # Compile as C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require C++20 support.

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

# Enable OpenMP by adding the compiler flag -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

# Collect all executables (can be expanded easily)
set(SOURCES
  bench.cpp
  controlling_timers.cpp
  custom_stat.cpp
  fast.cpp
  main.cpp
  manual_timing.cpp
  prevent_opt.cpp
  timer.cpp
) # Add more source files here

# Loop over each source file to create an executable
foreach(SRC ${SOURCES})
  get_filename_component(EXE_NAME ${SRC} NAME_WE) # Extract name without extension
  add_executable(${EXE_NAME} ${SRC}) # Create executable
  target_link_libraries(${EXE_NAME} PRIVATE benchmark::benchmark) # Link with Google Benchmark
endforeach()
