constexpr bool isPrime(unsigned int p) {
  for (unsigned int d=2; d<=p/2;++d) {
    if (p % d == 0) {
      return false;
    }
  }
  return p > 1;
}

int main() {
  static_assert(!isPrime(9), "run at compile time");
  return 0;
}
