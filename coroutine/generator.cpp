#include <coroutine>
#include <vector>
#include <iostream>

// Interleaving vectors
struct Generator {
  struct promise_type {
    int val_{};

    Generator get_return_object() { return Generator{this}; }

    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    std::suspend_always yield_value(int v) {
      val_ = v;
      return {};
    }

    void unhandled_exception() {}
  };

  using Handle = std::coroutine_handle<promise_type>;
  Handle coro_hdl_;

  explicit Generator(promise_type *p) : coro_hdl_{Handle::from_promise(*p)} {}

  Generator(Generator &&rhs)
      : coro_hdl_{std::exchange(rhs.coro_hdl_, nullptr)} {}

  ~Generator() {
    if (coro_hdl_) {
      coro_hdl_.destroy();
    }
  }

  int value() const { return coro_hdl_.promise().val_; }

  bool finished() const { return coro_hdl_.done(); }

  void resume() {
    if (!finished()) {
      coro_hdl_.resume();
    }
  }

  struct sentinel {};
  struct iterator {
    Handle coro_hdl_;

    bool operator==(sentinel) const { return coro_hdl_.done(); }

    iterator &operator++() {
      coro_hdl_.resume();
      return *this;
    }

    int operator*() const { return coro_hdl_.promise().val_; }
  };

  iterator begin() { return {coro_hdl_}; }
  sentinel end() { return {}; }
};

Generator interleaved(std::vector<int> a, std::vector<int> b) {
  auto lamb = [](std::vector<int> &v) -> Generator {
    for (const auto &e : v) {
      co_yield e;
    }
  };

  auto x = lamb(a);
  auto y = lamb(b);

  while (!x.finished() || !y.finished()) {
    if (!x.finished()) {
      co_yield x.value();
      x.resume();
    }
    if (!y.finished()) {
      co_yield y.value();
      y.resume();
    }
  }
}

int main() {
  std::vector a{2, 4, 6, 8};
  std::vector b{3, 5, 7, 9};
  Generator g{interleaved(std::move(a), std::move(b))};

  for (const auto &e : g) {
    std::cout << e << '\n';
  }
}