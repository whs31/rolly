#pragma once

#include <string>
#include <typeinfo>
#include <leaf/global/definitions.h>

namespace leaf::utils
{
  using std::string;

  namespace detail
  {
    LEAF_EXPORT [[nodiscard]] auto demangle(const char* name) -> string;
  }

  template<typename T>
  [[nodiscard]] auto type_name() -> string { return detail::demangle(typeid(T).name()); }
}