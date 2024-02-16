#pragma once

#include <memory>

namespace sim::utils
{
  using std::unique_ptr;
  using std::shared_ptr;

  template<class T, class... Args>
    auto make_unique_nothrow(Args &&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) -> unique_ptr<T> {
    // ReSharper disable once CppSmartPointerVsMakeFunction
    return unique_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
  }

  template<class T, class... Args>
  auto make_shared_nothrow(Args &&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) -> shared_ptr<T> {
    // ReSharper disable once CppSmartPointerVsMakeFunction
    return shared_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
  }
}