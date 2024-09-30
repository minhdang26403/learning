#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <thread>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

enum class parse_error { invalid_input, overflow };

auto parse_number(std::string_view& str) -> std::expected<double, parse_error> {
  const char* begin = str.data();
  char* end;
  double retval = std::strtod(begin, &end);

  if (begin == end)
    return std::unexpected(parse_error::invalid_input);
  else if (std::isinf(retval))
    return std::unexpected(parse_error::overflow);

  str.remove_prefix(end - begin);
  return retval;
}

int main() {
  fs::remove("output.txt");

  // Create a file stream object
  std::ofstream outputFile;

  // Open the file named "output.txt"
  outputFile.open("output.txt");

  char buffer[64] = {};


  std::thread t1{[&] {
    std::memcpy(&buffer[0], "hello", 5);
    // std::this_thread::sleep_for(1s)
    outputFile.write(buffer, sizeof(buffer));
  }};

  std::thread t2{[&] {
    std::this_thread::sleep_for(100ms);
    std::memcpy(&buffer[55], "world", 5);
  }};

  t1.join();
  t2.join();

  outputFile.close();

  return 0;
}
