#pragma once

#include <type_traits>

namespace leaf::utils {
  template <typename T>
  concept Enum = std::is_enum_v<T>;

  template <Enum T> constexpr auto to_underlying(T t) noexcept -> std::underlying_type_t<T>
  {
    return static_cast<std::underlying_type_t<T>>(t);
  }
} // namespace leaf::utils