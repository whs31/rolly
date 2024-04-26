#pragma once

#include <leaf/global.h>

namespace leaf::conversion {
  using std::string;
  using std::string_view;

  template <typename From, typename To> [[nodiscard]] auto into(From) -> To
  {
    throw std::runtime_error("unimplemented conversion");
  }

  template <typename T> [[nodiscard]] auto from_str(string_view) -> T
  {
    throw std::runtime_error("unimplemented conversion");
  }

  template <typename T> [[nodiscard]] auto try_from_str(string_view) -> expected<T, string>
  {
    throw std::runtime_error("unimplemented conversion");
  }

  template <typename T> [[nodiscard]] auto as_str(T) -> string
  {
    throw std::runtime_error("unimplemented conversion");
  }

  template <typename T> [[nodiscard]] auto try_as_str(T) -> string
  {
    throw std::runtime_error("unimplemented conversion");
  }
} // namespace leaf::conversion