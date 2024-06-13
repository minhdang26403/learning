#include <mutex>
#include <string>

class some_data {
 public:
  void do_something();

 private:
  int a;
  std::string b;
};

class data_wrapper {
 public:
  template <typename Function>
  void process_data(Function func) {
    std::lock_guard<std::mutex> l(m);
    // pass protected data to user-supplied function
    func(data);
  }

 private:
  some_data data;
  std::mutex m;
};

some_data* unprotected;

void malicious_function(some_data& protected_data) {
  unprotected = &protected_data;
}

data_wrapper x;
void foo() {
  // pass in a malicious function
  x.process_data(malicious_function);
  // unprotected access to protected data
  unprotected->do_something();
}
