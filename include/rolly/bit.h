#pragma once

#include "global.h"
#include "concepts/num.h"

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcount __popcnt
#endif  // _MSC_VER

namespace rolly {
  /**
   * @brief Inline namespace for math/bit utilities.
   */
  inline namespace math {
    /**
     * @brief Returns the number of 1 bits in the value.
     * @tparam T Type of the value. Must be an unsigned integral type.
     * @param x Value to count the number of 1 bits in.
     * @return The number of 1 bits in the value.
     */
    template <___concept___(std::integral) T ___sfinae_requirement___(std::is_integral_v<T>)>
    [[nodiscard]] constexpr int popcount(T x) noexcept {
      return __builtin_popcount(x);
    }

    /**
     * @brief Checks if the value is an integral power of two.
     * @tparam T Type of the value. Must be an integral type.
     * @param x Value to check.
     * @return True if the value is an integral power of two, false otherwise.
     * @see is_pow2
     */
    template <___concept___(std::integral) T ___sfinae_requirement___(std::is_integral_v<T>)>
    [[nodiscard]] constexpr bool has_single_bit(T x) noexcept {
      return popcount(static_cast<unsigned>(x)) == 1;
    }

    /**
     * @brief Checks if the value is an integral power of two.
     * @details This is an alias for @ref has_single_bit function.
     * @tparam T Type of the value. Must be an integral type.
     * @param x Value to check.
     * @return True if the value is an integral power of two, false otherwise.
     * @see has_single_bit
     */
    template <___concept___(std::integral) T ___sfinae_requirement___(std::is_integral_v<T>)>
    [[nodiscard]] constexpr bool is_pow2(T x) noexcept {
      return x and not (x bitand (x - 1));
    }

    /**
     * @brief Returns the next power of two of the value.
     * @tparam T Type of the value. Must be an unsigned integral type.
     * @param x Value.
     * @return The next power of two of the value.
     */
    template <___concept___(std::unsigned_integral) T ___sfinae_requirement___(std::is_unsigned_v<T>)>
    [[nodiscard]] constexpr T bit_ceil(T x) noexcept {
      x--;
      x |= x >> 1;
      x |= x >> 2;
      x |= x >> 4;
      x |= x >> 8;
      x |= x >> 16;
      return x + 1;
    }
  }  // namespace math
}  // namespace rolly
