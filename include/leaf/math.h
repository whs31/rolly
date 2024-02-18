#pragma once

#include <cmath>
#include <limits>
#include <leaf/global.h>
#include <leaf/traits.h>

namespace leaf
{
  template<traits::AnyNumber T>
  [[nodiscard]] constexpr auto eq(T a, T b) -> bool
  {
    using std::abs;
    using std::numeric_limits;
    return abs(a - b) <= numeric_limits<T>::epsilon();
  }

  template<traits::AnyNumber T>
  [[nodiscard]] constexpr auto null(T num) -> bool { return eq(num, 0); }
}