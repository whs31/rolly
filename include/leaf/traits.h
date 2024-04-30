#pragma once

#include <concepts>

namespace leaf::traits {
  template <typename T>
  concept FloatingPoint = std::floating_point<T>;

  template <typename T>
  concept Number = std::integral<T>;

  template <typename T>
  concept AnyNumber = Number<T> or FloatingPoint<T>;

  template <typename T>
  concept Array = std::is_array_v<T>;

  template <typename T>
  concept Object = std::is_object_v<T>;

  template <typename T>
  concept Function = std::is_function_v<T>;
} // namespace leaf::traits