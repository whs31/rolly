#pragma once

#include <string>
#include <typeinfo>

namespace leaf::utils
{
  using std::string;

  namespace detail
  {
    [[nodiscard]] auto demangle(const char* name) -> string;
  }

  template<typename T>
  [[nodiscard]] auto type_name() -> string { return detail::demangle(typeid(T).name()); }
}