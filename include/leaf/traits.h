#pragma once

#include <concepts>

namespace leaf::traits {
  template <typename T>
  concept FloatingPoint = std::floating_point<T>;

  template <typename T>
  concept Number = std::integral<T>;

  template <typename T>
  concept AnyNumber = Number<T> or FloatingPoint<T>;
} // namespace leaf::traits