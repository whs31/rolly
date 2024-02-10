#pragma once

#include <concepts>

namespace simkernel::traits
{
  template<typename T>
  concept FloatingPoint = std::floating_point<T>;

  template<typename T>
  concept Number = std::integral<T>;

  template<typename T>
  concept AnyNumber = Number<T> or FloatingPoint<T>;
}