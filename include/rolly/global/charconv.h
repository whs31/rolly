#pragma once

#include <charconv>
#include <string>
#include <string_view>

namespace rolly {
  /**
   * @brief Function prototype for converting C++ types to owned strings.
   * @tparam T Type to convert
   * @tparam C Character type
   * @param value Constant reference to the type to convert
   * @return Owned string representation of the type
   */
  template <typename T, typename C = char>
  [[nodiscard]] inline std::basic_string<C> to_string(T const& value) = delete;

  /**
   * @brief Function prototype for converting C++ types to non-owned strings.
   * @tparam T Type to convert
   * @tparam C Character type
   * @param value Constant reference to the type to convert
   * @return Non-owned string representation of the type
   */
  template <typename T, typename C = char>
  [[nodiscard]] constexpr inline std::basic_string_view<C> as_str(T const& type) = delete;

  /**
   * @brief Function prototype for converting strings to C++ types.
   * @tparam T Type to convert
   * @tparam C Character type
   * @param str Non-owned string representation of the type
   * @return Value of the converted type
   */
  template <typename T, typename C = char>
  [[nodiscard]] constexpr inline T from_str(std::basic_string_view<C> const& str) = delete;
}  // namespace rolly
