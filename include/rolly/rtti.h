#pragma once

#include <string>
#include <typeinfo>
#include "global/export.h"
#include "global/definitions.h"

namespace rolly
{
  /**
   * @brief Runtime type information-related functions and classes.
   */
  namespace rtti {} // namespace rtti
} // namespace rolly

namespace rolly::rtti
{
  /**
   * @brief Demangles C++ type name from string.
   * @details Confirmed support on:
   * <ul>
   *   <li>MSVC</li>
   *   <li>Clang</li>
   *   <li>GCC</li>
   * </ul>
   * @param name C++ mangled type name string.
   * @return Demangled type name.
   * @note Requires RTTI to be enabled.
   * @see type_name
   */
  [[nodiscard]]
#ifndef ROLLY_DOC
  ___rolly_api___
#endif // ROLLY_DOC
  std::string demangle(char const* name);

  /**
   * @brief Returns C++ type name or signature from <b>typeid</b> object using RTTI.
   * @details Confirmed support on:
   * <ul>
   *   <li>MSVC</li>
   *   <li>Clang</li>
   *   <li>GCC</li>
   * </ul>
   *
   * Example usage:
   * @code {.cpp}
   * using namespace rolly::rtti;
   *
   * int main() {
   *   std::cout << type_name<int>(); // prints: 'int'
   *   std::cout << type_name<std::string>(); // prints: 'std::basic_string<char>'
   * }
   * @endcode
   * @return Demangled type name.
   * @note Requires RTTI to be enabled.
   * @see demangle
   */
  template <typename T>
  [[nodiscard]] std::string type_name() {
    return demangle(typeid(T).name());
  }
} // namespace rolly::rtti