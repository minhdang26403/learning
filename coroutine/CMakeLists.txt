cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For clang-tidy.
set(CMAKE_CXX_STANDARD 20) # Compile as C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require C++20 support.

project(LearnCppConcurrency VERSION 2024.1
  DESCRIPTION "Code examples to learn C++ concurrency"
  LANGUAGES C CXX)

# #####################################################################################################################
# COMPILER SETUP
# #####################################################################################################################

# Set the binary directory within the build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT DEFINED SANITIZER)
  set(SANITIZER thread)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fsanitize=${SANITIZER} -fno-omit-frame-pointer -fno-optimize-sibling-calls")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/f8d7d77.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

# Executables
add_executable(coroutine coroutine.cpp)
add_executable(generator generator.cpp)
add_executable(async_reset_event async_reset_event.cpp)
add_executable(symmetric_transfer symmetric_transfer.cpp)

# # Unit tests
# set(TESTS
#   coroutine_test
# )

# # Loop through each test
# foreach(TEST_NAME ${TESTS})
#   # Add ".cpp" suffix to the test name
#   set(TEST_NAME_CPP "${TEST_NAME}.cpp")

#   # Create executable from the test source file
#   add_executable(${TEST_NAME} ${TEST_NAME_CPP})

#   # Link the test with gtest_main
#   target_link_libraries(${TEST_NAME} GTest::gtest_main)

#   # Discover tests for each executable
#   gtest_discover_tests(${TEST_NAME})
# endforeach()
