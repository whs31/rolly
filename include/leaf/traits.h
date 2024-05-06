#pragma once

#include <concepts>
#include <type_traits>
#include <string>
#include <string_view>
#include <fmt/format.h>

class QString;
class QStringView;

namespace leaf::trait
{
  inline namespace c
  {
    template <typename T>
    concept Num = std::integral<T> or std::floating_point<T>;

    template <typename T>
    concept Array = std::is_array_v<T>;

    template <typename T>
    concept Object = std::is_object_v<T>;

    template <typename T>
    concept Fn = std::is_function_v<T>;

    template <typename T>
    concept Ptr = std::is_pointer_v<T>;

    template <typename T>
    concept AnyStringSlice = std::is_same_v<T, std::string_view>
      or std::is_same_v<T, std::wstring_view>
      or std::is_same_v<T, std::u16string_view>
      or std::is_same_v<T, std::u32string_view>
      or std::is_same_v<T, QStringView>;

    template <typename T>
    concept AnyString = std::is_same_v<T, std::string>
      or std::is_same_v<T, QString>
      or std::is_same_v<T, std::wstring>
      or std::is_same_v<T, std::u16string>
      or std::is_same_v<T, std::u32string>
      or AnyStringSlice<T>;
  }

  namespace i
  {
    template <c::AnyString T>
    struct Display
    {
      [[nodiscard]] virtual auto display() const -> T = 0;
      [[nodiscard]] virtual inline auto to_string() const -> T { return this->display(); };
      [[nodiscard]] virtual inline auto as_str() const -> T { return this->display(); };
    };

    template <typename T>
    requires AnyString<T> or AnyStringSlice<T>
    struct Debug
    {
      [[nodiscard]] virtual auto debug_str() const -> T = 0;
    };
  }

  inline namespace c
  {
    template <typename T, typename S>
    concept Display = std::is_base_of_v<trait::i::Display<S>, T>;

    template <typename T, typename S>
    concept Debug = std::is_base_of_v<trait::i::Debug<S>, T>;
  }
}

#define implements_display(type, string_type)                                                         \
template <>                                                                                           \
struct formatter<type> {                                                                              \
  static_assert(leaf::trait::c::Display<type, string_type>, "fmt_implements_display: type must implement i::Display");  \
  template <typename ParseContext>                                                                    \
  constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }                                     \
                                                                                                      \
  template <typename FormatContext>                                                                   \
  static auto format(const type& v, FormatContext& ctx) {                                             \
      return fmt::format_to(ctx.out(), "{}", v.to_string());                                          \
  }                                                                                                   \
};

#define implements_debug(type, string_type)                                                           \                                                           \
template <>                                                                                           \
struct formatter<type> {                                                                              \
  static_assert(leaf::trait::c::Debug<type, string_type>, "fmt_implements_debug: type must implement i::Debug");   \
  template <typename ParseContext>                                                                    \
  constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }                                     \
                                                                                                      \
  template <typename FormatContext>                                                                   \
  static auto format(const type& v, FormatContext& ctx) {                                             \
      return fmt::format_to(ctx.out(), "{}", v.debug_str());                                          \
  }                                                                                                   \
};