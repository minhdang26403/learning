enable_testing()

add_executable(
  hello_test
  hello_test.cc)

target_link_libraries(
  hello_test
  GTest::gtest_main
  hello
  blah
)

include(GoogleTest)
gtest_discover_tests(hello_test)
