#pragma once

#include <string>
#include <typeinfo>

namespace simkernel::utils
{
  using std::string;

  namespace detail
  {
    [[nodiscard]] auto demangle(const char* name) -> string;
  }

  template<typename T>
  [[nodiscard]] auto type_name(const T& t) -> string { return detail::demangle(typeid(t).name()); }
}