#pragma once

#include <cmath>
#include "numbers.h"

namespace rolly {
  /**
   * @brief Inline namespace for mathematical functions.
   */
  inline namespace math {
    template <___concept___(std::floating_point) T ___sfinae_requirement___(std::is_floating_point_v<T>)>
    [[nodiscard]] constexpr inline T floor(T val) {
      auto const val_int = static_cast<i64>(val);
      auto const fval_int = static_cast<T>(val_int);
      return (val >= T(0) ? fval_int : (val == fval_int ? val : fval_int - T(1)));
    }

    template <typename T>
    constexpr T min(T&& v) {
      return std::forward<T>(v);
    }

    template <typename T, typename... Args>
    constexpr T min(T const& v1, T const& v2, Args const&... args) {
      return v2 < v1 ? min(v2, args...) : min(v1, args...);
    }

    template <typename T>
    [[maybe_unused]] constexpr T max(T&& v) {
      return std::forward<T>(v);
    }

    template <typename T, typename... Args>
    constexpr T max(T const& v1, T const& v2, Args const&... args) {
      return v2 > v1 ? max(v2, args...) : max(v1, args...);
    }

    template <typename T>
    constexpr T abs(T const& v) {
      return v < T(0) ? -v : v;
    }

    constexpr f32 fmod(f32 const x, f32 const y) { return x - y * math::floor(x / y); }

    /**
     * @brief Converts degrees to radians.
     * @tparam T Underlying type. Must be an integral or floating point type.
     * @param degrees Degree value.
     * @return Radian value.
     */
    template <___concept___(concepts::num) T ___sfinae_requirement___(is_num_v<T>)>  // NOLINT(*-use-constraints)
    [[nodiscard]] constexpr T to_radians(T degrees) noexcept {
      return degrees * numbers::pi_v<T> / static_cast<T>(180.0);
    }

    /**
     * @brief Converts radians to degrees.
     * @tparam T Underlying type. Must be an integral or floating point type.
     * @param radians Radian value.
     * @return Degree value.
     */
    template <___concept___(concepts::num) T ___sfinae_requirement___(is_num_v<T>)>  // NOLINT(*-use-constraints)
    [[nodiscard]] constexpr T to_degrees(T radians) noexcept {
      return radians * static_cast<T>(180.0) / numbers::pi_v<T>;
    }

    /**
     * @brief Returns true if numbers are approximately equal within given epsilon.
     * @details Compares floating point values using formula <tt>|a - b| <= epsilon * epsilon_factor</tt>.
     * @tparam T Number type
     * @param a First number
     * @param b Second number
     * @param epsilon Epsilon factor for comparison. Default is <tt>std::numeric_limits<T>::epsilon()</tt>.
     * @return True if numbers are approximately equal within given epsilon.
     * @see is_null
     */
    template <typename T>
    [[nodiscard]] constexpr bool approx_eq(T a, T b, T epsilon = std::numeric_limits<T>::epsilon()) {
      if constexpr(std::is_floating_point_v<T>)
        return std::abs(a - b) <= epsilon;
      else
        return a == b;
    }

    /**
     * @brief Returns true if number is approximately zero within given epsilon.
     * @details Compares floating point values using formula <tt>|a| <= epsilon * epsilon_factor</tt>.
     * @tparam T Number type
     * @param a Number
     * @param epsilon Epsilon factor for comparison. Default is <tt>std::numeric_limits<T>::epsilon()</tt>.
     * @return True if number is zero within given epsilon.
     * @see approx_eq
     */
    template <typename T>
    [[nodiscard]] constexpr bool is_null(T a, T epsilon = std::numeric_limits<T>::epsilon()) {
      return approx_eq(a, static_cast<T>(0), epsilon);
    }
  }  // namespace math
}  // namespace rolly
