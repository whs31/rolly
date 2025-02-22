#pragma once

#include <limits>
#include <type_traits>
#include <rll/global.h>
#include <rll/concepts/num.h>

namespace rll {
  /**
   * @brief Number numeric constants.
   */
  namespace numbers {}  // namespace numbers
}  // namespace rll

namespace rll::numbers {
  template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
  inline constexpr T pi_v = 3.141592653589793238462643383279502884;  // NOLINT(*-use-std-numbers)

  template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
  inline constexpr T e_v = 2.718281828459045235360287471352662497;   // NOLINT(*-use-std-numbers)

  /**
   * @brief Pi constant (π) as defined in <a href="https://en.wikipedia.org/wiki/Pi">Wikipedia</a>.
   * @details Represented as 32-bit floating point number.
   */
  inline constexpr auto pi = pi_v<float>;

  /**
   * @brief E constant as defined in <a
   * href="https://en.wikipedia.org/wiki/E_(mathematical_constant)">Wikipedia</a>.
   * @details Represented as 32-bit floating point number.
   */
  inline constexpr auto e = e_v<float>;
}  // namespace rll::numbers
