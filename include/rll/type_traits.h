#pragma once

#include <type_traits>

namespace rll {
  /**
   * @brief Remove all cv qualifiers, references and pointers from a type.
   * @details If the type T is a reference type, provides the member typedef type which is the type
   * referred to by T with its topmost cv-qualifiers removed. Otherwise type is T with its topmost
   * cv-qualifiers removed.
   *
   * This is a backport of the C++20 <tt>std::remove_cvref</tt> type trait.
   * @tparam T Type to remove qualifiers from.
   * @sa https://en.cppreference.com/w/cpp/types/remove_cvref
   */
  template <typename T>
  struct remove_cvref {
    /**
     * @brief The type referred by T or T itself if it is not a reference, with top-level
     * cv-qualifiers removed.
     */
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
  };

  /**
   * @brief The type referred by T or T itself if it is not a reference, with top-level
   * cv-qualifiers removed.
   */
  template <typename T>
  using remove_cvref_t = typename remove_cvref<T>::type;

  /**
   * @brief Remove all cv qualifiers, references and pointers from a type.
   * @details For example:
   * <ul>
   *    <li><tt>std::string&</tt> becomes <tt>std::string</tt></li>
   *    <li><tt>const int*</tt> becomes <tt>int</tt></li>
   *    <li><tt>int**</tt> becomes <tt>int</tt></li>
   *    <li><tt>const volatile std::string**</tt> becomes <tt>std::string</tt></li>
   * </ul>
   * @tparam T The type to remove qualifiers from.
   * @see plain_type_t
   */
  template <typename T>
  struct plain_type {
    /**
     * @brief The type without qualifiers, references and pointers.
     */
    using type = remove_cvref_t<std::remove_pointer_t<T>>;

    /**
     * @brief The type without qualifiers, references and pointers with a const qualifier.
     */
    using const_type = std::add_const_t<type>;
  };

  /**
   * @brief Remove all cv qualifiers, references and pointers from a type.
   * @see plain_type
   */
  template <typename T>
  using plain_type_t = typename plain_type<T>::type;
}  // namespace rll
