#include <atomic>
#include <chrono>
#include <coroutine>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

class async_manual_reset_event {
 public:
  async_manual_reset_event(bool initially_set = false) noexcept
      : state_(initially_set ? this : nullptr) {}

  // No copying/moving
  async_manual_reset_event(const async_manual_reset_event&) = delete;
  async_manual_reset_event& operator=(const async_manual_reset_event&) = delete;
  async_manual_reset_event(async_manual_reset_event&&) = delete;
  async_manual_reset_event& operator=(async_manual_reset_event&&) = delete;

  bool is_set() const noexcept { return state_.load(std::memory_order_acquire) == this; }

  struct awaiter {
    awaiter(const async_manual_reset_event& event) : event_(event) {}

    bool await_ready() const noexcept { return event_.is_set(); }

    bool await_suspend(std::coroutine_handle<> awaiting_coroutine) noexcept {
      // Special m_state value that indicates the event is in the 'set' state.
      const void* const set_state = this;

      // Remember the handle of the awaiting coroutine.
      awaiting_coroutine_ = awaiting_coroutine;

      // Try to atomically push this awaiter onto the front of the list.
      void* old_value = event_.state_.load(std::memory_order_acquire);
      do {
        // Resume immediately if already in 'set' state.
        if (old_value == set_state) {
          return false;
        }

        // Update linked list to point at current head.
        next_ = static_cast<awaiter*>(old_value);

        // Finally, try to swap the old list head, inserting this awaiter
        // as the new list head.
      } while (event_.state_.compare_exchange_weak(old_value, this,
                                                   std::memory_order_release,
                                                   std::memory_order_acquire));

      // Successfully enqueued. Remain suspended.
      return true;
    }

    void await_resume() noexcept {}

   private:
    friend class async_manual_reset_event;

    const async_manual_reset_event& event_;
    std::coroutine_handle<> awaiting_coroutine_;
    awaiter* next_;
  };

  awaiter operator co_await() const noexcept { return awaiter{*this}; }

  void set() noexcept {
    void* old_value = state_.exchange(this, std::memory_order_acq_rel);
    if (old_value != this) {
      // Wasn't already in 'set' state.
      // Treat old value as head of a linked-list of waiters
      // which we have now acquired and need to resume.
      auto waiters = static_cast<awaiter*>(old_value);
      while (waiters != nullptr) {
        // Read next_ before resuming the coroutine as resuming
        // the coroutine will likely destroy the awaiter object.
        auto next = waiters->next_;
        waiters->awaiting_coroutine_.resume();
        waiters = next;
      }
    }
  }

  void reset() noexcept {
    void* old_value = this;
    state_.compare_exchange_strong(old_value, nullptr,
                                   std::memory_order_release);
  }

 private:
  // - 'this' => set state
  // - otherwise => not set, head of linked list of awaiter*.
  mutable std::atomic<void*> state_;
};

struct task {
  struct promise_type {
    task get_return_object() { return task{this}; }

    std::suspend_never initial_suspend() noexcept { return {}; }

    std::suspend_always final_suspend() noexcept { return {}; }

    void return_void() {}

    void unhandled_exception() {}
  };

  using Handle = std::coroutine_handle<promise_type>;
  Handle coro_hdl_;

  explicit task(promise_type* p) : coro_hdl_{Handle::from_promise(*p)} {}

  task(task&& rhs) : coro_hdl_{std::exchange(rhs.coro_hdl_, nullptr)} {}

  ~task() {
    if (coro_hdl_) {
      coro_hdl_.destroy();
    }
  }

  bool finished() const { return coro_hdl_.done(); }

  void resume() {
    if (!finished()) {
      coro_hdl_.resume();
    }
  }
};

void producer(async_manual_reset_event& event, int& value) {
  std::this_thread::sleep_for(1s);
  value = 26;
  // Publish the value by setting the event.
  event.set();
}

int main() {
  async_manual_reset_event event;
  int value;
  producer(event, value);

  int num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  std::mutex m;
  for (int i = 0; i < num_threads; i++) {
    // consumer threads
    threads.emplace_back([&]() -> task {
      // Wait until the event is signalled by call to event.set()
      // in the producer() function.
      co_await event;
      std::scoped_lock lk(m);
      std::cout << "Get value " << value << '\n';
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }

  return 0;
}
