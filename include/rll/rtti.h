#pragma once

#include <string>
#include <typeinfo>
#include <rll/global/export.h>
#include <rll/global/definitions.h>

namespace rll {
  /**
   * @brief Runtime type information-related functions and classes.
   */
  namespace rtti {}  // namespace rtti
}  // namespace rll

namespace rll::rtti {
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
#ifndef RLL_DOC
  RLL_API
#endif  // RLL_DOC
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
   * using namespace rll::rtti;
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
}  // namespace rll::rtti
