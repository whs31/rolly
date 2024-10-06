#pragma once

#include <string>
#include <string_view>
#include <fmt/format.h>
#include "global.h"
#include "type_traits.h"
#include "rtti.h"
#include "exception/serialization_error.h"
#include "serialization/tags.h"

namespace rolly::serialization
{
  template <typename F, typename T, typename C = char>
  [[maybe_unused]] [[nodiscard]] std::basic_string<C> serialize(T const& value) = delete;

  template <typename F, typename T, typename C = char>
  [[maybe_unused]] [[nodiscard]] T deserialize(std::basic_string<C> const& str) = delete;

#if defined(___rolly_cxx20___) || defined(ROLLY_DOC)
  template <typename F, typename T, typename C = char>
  concept serializable = requires(T const& t) {
    { serialize<F, T, C>(t) } -> std::convertible_to<std::basic_string<C>>;
  };

  template <typename F, typename T, typename C = char>
  concept deserializable = requires(std::basic_string<C> const& str) {
    { deserialize<F, T, C>(str) } -> std::convertible_to<T>;
  };

  template <typename F, typename T, typename C = char>
  concept serializable_and_deserializable = serializable<F, T, C> and deserializable<F, T, C>;
#endif // defined(___rolly_cxx20___)
} // namespace rolly::serialization