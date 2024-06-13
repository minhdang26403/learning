#pragma once

#include "future.h"
#include "packaged_task.h"
#include <type_traits>
#include <utility>

template <typename F, typename... Args>
Future<std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
Async(F &&f, Args &&...args) {
  // TODO: Add execution policy for `Async`. May need to implement an `Executor`
  // class for task scheduling
  using return_type =
      std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>;
  PackagedTask<return_type()> task{
      std::bind<return_type>(std::forward<F>(f), std::forward<Args>(args)...)};
  auto future = task.get_future();
  std::thread t(std::move(task));
  t.detach();

  return future;
}
