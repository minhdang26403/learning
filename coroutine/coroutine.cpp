#include <coroutine>
#include <iostream>
#include <string>
#include <future>

using namespace std::string_literals;

// Basic coroutines
struct Chat {
  struct promise_type {
    std::string msg_out_, msg_in_;

    void unhandled_exception() noexcept {}

    Chat get_return_object() { return Chat(this); }

    std::suspend_always initial_suspend() noexcept { return {}; }

    std::suspend_always yield_value(std::string msg) noexcept {
      msg_out_ = std::move(msg);
      return {};
    }

    auto await_transform(std::string) noexcept {
      struct awaiter {
        promise_type &pt;
        constexpr bool await_ready() const noexcept { return true; }
        std::string await_resume() const noexcept {
          return std::move(pt.msg_in_);
        }
        void await_suspend(std::coroutine_handle<>) const noexcept {}
      };

      return awaiter{*this};
    }

    void return_value(std::string msg) noexcept { msg_out_ = std::move(msg); }

    std::suspend_always final_suspend() noexcept { return {}; }
  };

  using Handle = std::coroutine_handle<promise_type>;
  Handle coro_hdl_;

  explicit Chat(promise_type *p) : coro_hdl_{Handle::from_promise(*p)} {}
  Chat(Chat &&rhs) : coro_hdl_{std::exchange(rhs.coro_hdl_, nullptr)} {}

  ~Chat() {
    if (coro_hdl_) {
      coro_hdl_.destroy();
    }
  }

  std::string listen() {
    if (!coro_hdl_.done()) {
      coro_hdl_.resume();
    }
    return std::move(coro_hdl_.promise().msg_out_);
  }

  void answer(std::string msg) {
    coro_hdl_.promise().msg_in_ = msg;
    if (!coro_hdl_.done()) {
      coro_hdl_.resume();
    }
  }
};

Chat Fun() {
  co_yield "Hello!\n"s;

  std::cout << co_await std::string{};

  co_return "Here!\n"s;
}

int main() {
  Chat chat = Fun();
  std::cout << chat.listen();
  chat.answer("Where are you?\n"s);
  std::cout << chat.listen();

  return 0;
}
