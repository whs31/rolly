#pragma once

#include <cmath>
#include <limits>
#include <numbers>
#include <leaf/global.h>
#include <leaf/traits.h>

namespace leaf {
  template <traits::AnyNumber T> [[nodiscard]] constexpr auto eq(T a, T b) -> bool
  {
    using std::abs;
    using std::numeric_limits;
    return abs(a - b) <= numeric_limits<T>::epsilon();
  }

  template <traits::AnyNumber T> [[nodiscard]] constexpr auto null(T num) -> bool
  {
    return eq(num, 0);
  }

  template <traits::AnyNumber T> [[nodiscard]] constexpr auto to_radians(T deg) -> T
  {
    using std::numbers::pi_v;
    return static_cast<T>(deg * pi_v<T> / 180.0);
  }

  template <traits::AnyNumber T> [[nodiscard]] constexpr auto to_degrees(T rad) -> T
  {
    using std::numbers::pi_v;
    return static_cast<T>(rad * 180.0 / pi_v<T>);
  }

  template <traits::FloatingPoint T> [[nodiscard]] constexpr auto logp(T base, T num) -> T
  {
    using std::log;
    return log(num) / log(base);
  }
} // namespace leaf