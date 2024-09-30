#include <string>

class BoolString {
 private:
  std::string value;

 public:
  BoolString(std::string const& s) : value(s) {}

  template <typename T = std::string>
  T get() const {
    return value;
  }
};

template <>
inline bool BoolString::get<bool>() const {
  return value == "true" || value == "1" || value == "on";
}

template<typename T>
constexpr T pi{3.14159};