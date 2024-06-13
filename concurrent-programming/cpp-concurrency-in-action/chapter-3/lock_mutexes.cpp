#include <mutex>

struct some_big_object {
  int a;
};

void swap(some_big_object& lhs, some_big_object& rhs) {
  std::swap(lhs.a, rhs.a);
}

class X {
 public:
  X(const some_big_object& sd) : some_detail(sd) {}

  friend void swap(X& lhs, X& rhs) {
    if (&lhs == &rhs) {
      return;
    }
    std::lock(lhs.m, rhs.m);
    std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
    std::swap(lhs.some_detail, rhs.some_detail);
  }
 private:
  some_big_object some_detail;
  std::mutex m;
};


int main() {
  some_big_object obj1{10};
  some_big_object obj2{11};
  X x1(obj1);
  X x2(obj2);
  swap(x1, x2);

  return 0;
}