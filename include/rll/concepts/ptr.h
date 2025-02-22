#pragma once

#include <type_traits>
#include <rll/global/definitions.h>

#if __cplusplus >= 202'002L
#  include <concepts>
#endif

namespace rll {
#if __cplusplus >= 202'002L || defined(RLL_DOC)
  /**
   * @brief Pointer concept.
   * @details Will yield <code>true</code> for any type that is a pointer or reference.
   *
   * Note that this concept will be evaluated to <code>false</code> for function pointers,
   * <code>std::nullptr_t</code> and smart pointers.
   * @tparam T Type to check.
   * @note Available only in C++20 mode.
   * @see c_ptr
   * @see smart_ptr
   * @see ptr_like
   */
  template <typename T>
  concept ptr = std::is_pointer_v<T>;

  /**
   * @brief Raw pointer concept.
   * @details Will yield <code>true</code> for any type that is a raw pointer <b>and not a
   * reference</b>.
   *
   * Note that this concept will be evaluated to <code>false</code> for function pointers,
   * <code>std::nullptr_t</code>, <b>references</b> and smart pointers.
   * @tparam T Type to check.
   * @note Available only in C++20 mode.
   * @see ptr
   * @see smart_ptr
   * @see ptr_like
   */
  template <typename T>
  concept c_ptr = std::is_pointer_v<T> and not std::is_reference_v<T>;

  /**
   * @brief Smart pointer concept.
   * @details Will yield <code>true</code> for any type that is a smart pointer.
   *
   * Requirements:
   * <ul>
   *   <li>Can be dereferenced</li>
   *   <li>Can be converted to <code>bool</code></li>
   *   <li>Operator <code>operator-></code> must return a pointer of the same type as the
   * dereferenced pointer</li> <li>Method <code>get</code> must return the same type as the
   * dereferenced pointer</li>
   * </ul>
   * @tparam T Type to check.
   * @note Available only in C++20 mode.
   * @see ptr
   * @see ptr_like
   */
  template <typename T>
  concept smart_ptr = requires(T t) {
    { *t };
    { static_cast<bool>(t) };
    { t.operator->() } -> std::convertible_to<decltype(&*t)>;
    { t.get() } -> std::convertible_to<decltype(&*t)>;
  };

  /**
   * @brief Concept that is true if T is a pointer or smart pointer.
   * @tparam T Type to check.
   * @note Available only in C++20 mode.
   * @see ptr
   * @see smart_ptr
   */
  template <typename T>
  concept ptr_like = ptr<T> or smart_ptr<T>;
#endif
}  // namespace rll
