#pragma once

#include <concepts>
#include <type_traits>
#include <string>
#include <string_view>
#include <fmt/format.h>
#include <lf/detail/types.h>

class QString;
class QStringView;

namespace lf
{
  /**
   * \brief Remove all cv qualifiers, references and pointers from a type.
   * \details E.g: <br> - <tt>std::string&</tt> becomes <tt>std::string</tt>;<br>
   * - <tt>const int*</tt> becomes <tt>int</tt>.
   * - <tt>int**</tt> becomes <tt>int</tt>.
   * - <tt>const volatile std::string**</tt> becomes <tt>std::string</tt>.
   * \note Use <tt>plain_type_t<T>::maybe_pointer_type</tt> instead of <tt>plain_type_t<T>::type</tt>
   * if you want to keep pointer, but remove cv and references.
   * \tparam T The type to remove qualifiers from.
   * \sa plain_type_t
   */
  template <typename T>
  struct plain_type
  {
    using type = std::remove_cvref_t<std::remove_pointer_t<T>>;
    using const_type = std::add_const_t<type>;
    using maybe_pointer_type = std::remove_cvref_t<type>;
  };

  /// \brief Remove all cv qualifiers, references and pointers from a type.
  template <typename T>
  using plain_type_t = typename plain_type<T>::type;
};

namespace lf::trait
{
  inline namespace c
  {
    template <typename T>
    concept Num = std::integral<plain_type_t<T>> or std::floating_point<plain_type_t<T>>;

    template <typename T>
    concept Array = std::is_array_v<T>;

    template <typename T>
    concept Object = std::is_object_v<plain_type_t<T>>;

    template <typename T>
    concept Fn = std::is_function_v<plain_type_t<T>>;

    template <typename T>
    concept Ptr = std::is_pointer_v<typename plain_type<T>::maybe_pointer_type>;

    template <typename T>
    concept Enum = std::is_enum_v<plain_type_t<T>>;

    template <typename T>
    concept AnyStringSlice = std::is_same_v<plain_type_t<T>, std::string_view>
      or std::is_same_v<plain_type_t<T>, std::wstring_view>
      or std::is_same_v<plain_type_t<T>, std::u16string_view>
      or std::is_same_v<plain_type_t<T>, std::u32string_view>
      or std::is_same_v<plain_type_t<T>, QStringView>;

    template <typename T>
    concept AnyString = std::is_same_v<plain_type_t<T>, std::string>
      or std::is_same_v<plain_type_t<T>, QString>
      or std::is_same_v<plain_type_t<T>, std::wstring>
      or std::is_same_v<plain_type_t<T>, std::u16string>
      or std::is_same_v<plain_type_t<T>, std::u32string>
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

    template <typename T>
    struct Into
    {
      [[nodiscard]] virtual auto into() const -> T = 0;
    };

    struct Serializable
    {
      [[nodiscard]] virtual auto serialize() const -> Result<std::string> = 0;
    };

    struct Deserializable
    {
      [[nodiscard]] virtual auto deserialize(std::string_view data) -> Result<> = 0;
    };
  };

  inline namespace c
  {
    template <typename T, typename S>
    concept Display = std::is_base_of_v<trait::i::Display<S>, T>;

    template <typename T, typename S>
    concept Debug = std::is_base_of_v<trait::i::Debug<S>, T>;

    template <typename T, typename S>
    concept Into = std::is_base_of_v<trait::i::Into<S>, T>;

    template <typename T>
    concept Serializable = std::is_base_of_v<trait::i::Serializable, T>;

    template <typename T>
    concept Deserializable = std::is_base_of_v<trait::i::Deserializable, T>;

    template <typename T>
    concept SerializableDeserializable = Serializable<T> and Deserializable<T>;
  }
}

#define implements_display(type, string_type)                                                         \
template <>                                                                                           \
struct formatter<type> {                                                                              \
  static_assert(lf::trait::c::Display<type, string_type>, "fmt_implements_display: type must implement i::Display");  \
  template <typename ParseContext>                                                                    \
  constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }                                     \
                                                                                                      \
  template <typename FormatContext>                                                                   \
  static auto format(const type& v, FormatContext& ctx) {                                             \
      return fmt::format_to(ctx.out(), "{}", v.to_string());                                          \
  }                                                                                                   \
};

#define implements_debug(type, string_type)                                                                                                                      \
template <>                                                                                           \
struct formatter<type> {                                                                              \
  static_assert(lf::trait::c::Debug<type, string_type>, "fmt_implements_debug: type must implement i::Debug");   \
  template <typename ParseContext>                                                                    \
  constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }                                     \
                                                                                                      \
  template <typename FormatContext>                                                                   \
  static auto format(const type& v, FormatContext& ctx) {                                             \
      return fmt::format_to(ctx.out(), "{}", v.debug_str());                                          \
  }                                                                                                   \
};