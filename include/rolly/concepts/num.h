#pragma once

#include "../global/definitions.h"
#include "../type_traits.h"

#if defined(___rolly_cxx20___)
#  include <concepts>
#endif  // defined(___rolly_cxx20___)

namespace rolly {
  /**
   * @brief Concepts namespace.
   * @note Available only in C++20 mode.
   */
  namespace concepts {}  // namespace concepts
}  // namespace rolly

namespace rolly::concepts {
#if defined(___rolly_cxx20___) || defined(ROLLY_DOC)
  /**
   * @brief Concept that is true if T is a number.
   * @details Will yield <code>true</code> for any type that is an integral or floating point type,
   * except for <code>bool</code> and <code>std::nullptr_t</code>.
   * @note This concept will also yield <code>true</code> for references to numbers.
   * @note Available only in C++20 mode. Use @ref rolly::is_num or @ref rolly::is_num_v in C++17
   * mode instead
   * @tparam T Type to check.
   */
  template <typename T>
  concept num = std::integral<plain_type_t<T>>
             or std::floating_point<plain_type_t<T>> and (not std::is_same_v<plain_type_t<T>, bool>)
                  and (not std::is_same_v<plain_type_t<T>, std::nullptr_t>);
#endif  // defined(___rolly_cxx20___) || defined(ROLLY_DOC)
}  // namespace rolly::concepts

namespace rolly {
  /**
   * @brief Type trait for checking if T is a number.
   * @details Will yield <code>true</code> for any type that is an integral or floating point type,
   * @note Use @ref rolly::concepts::num instead if you are using C++20 or newer.
   * @tparam T Type to check.
   * @see is_num_v
   */
  template <typename T>
  struct is_num
    : std::integral_constant<
        bool,
        std::is_integral_v<plain_type_t<T>> or std::is_floating_point_v<plain_type_t<T>>> {};

  /**
   * @brief Helper type trait for checking if T is a number.
   * @tparam T Type to check.
   * @relates is_num
   */
  template <typename T>
  inline constexpr bool is_num_v = is_num<T>::value;
}  // namespace rolly
