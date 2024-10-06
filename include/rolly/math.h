#pragma once

#include <cmath>
#include "numbers.h"

namespace rolly
{
  /**
   * @brief Inline namespace for mathematical functions.
   */
  inline namespace math
  {
    /**
     * @brief Converts degrees to radians.
     * @tparam T Underlying type. Must be an integral or floating point type.
     * @param degrees Degree value.
     * @return Radian value.
     */
    template <___concept___(concepts::num) T ___sfinae_requirement___(std::is_integral_v<T> or std::is_floating_point_v<T>)> // NOLINT(*-use-constraints)
    [[nodiscard]] constexpr T to_radians(T degrees) noexcept {
      return degrees * numbers::pi_v<T> / static_cast<T>(180.0);
    }

    /**
     * @brief Converts radians to degrees.
     * @tparam T Underlying type. Must be an integral or floating point type.
     * @param radians Radian value.
     * @return Degree value.
     */
    template <___concept___(concepts::num) T ___sfinae_requirement___(std::is_integral_v<T> or std::is_floating_point_v<T>)> // NOLINT(*-use-constraints)
    [[nodiscard]] constexpr T to_degrees(T radians) noexcept {
      return radians * static_cast<T>(180.0) / numbers::pi_v<T>;
    }
  } // namespace math
} // namespace rolly