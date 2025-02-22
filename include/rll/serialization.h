#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <fmt/format.h>
#include <rll/global.h>
#include <rll/type_traits.h>
#include <rll/result.h>
#include <rll/serialization/tags.h>

namespace rll {
  template <typename T, typename F, typename C = char, typename = void>
  struct serializer {
    [[nodiscard]] static result<> serialize(T const& value, std::basic_ostream<char, F>& stream) =
      delete;
    [[nodiscard]] static result<T> deserialize(std::basic_istream<char, F>& stream) = delete;
  };

  template <typename T, typename F, typename R, typename C = char, typename = void>
  struct partial_serializer {
    [[nodiscard]] static result<R> serialize(T const& value) = delete;
    [[nodiscard]] static result<T> deserialize(R const& value) = delete;
  };

  template <typename, typename, typename = void>
  struct is_serializable : std::false_type {};

  template <typename, typename, typename, typename = void>
  struct is_partially_serializable : std::false_type {};

  template <typename T, typename F>
  struct is_serializable<
    T,
    F,
    std::void_t<decltype(serializer<T, F>::serialize), decltype(serializer<T, F>::deserialize)>>
    : std::true_type {};

  template <typename T, typename F, typename R>
  struct is_partially_serializable<
    T,
    F,
    R,
    std::void_t<
      decltype(partial_serializer<T, F, R>::serialize),
      decltype(partial_serializer<T, F, R>::deserialize)>> : std::true_type {};
}  // namespace rll
