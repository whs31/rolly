#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <fmt/format.h>
#include <tl/expected.hpp>

namespace floppy // NOLINT(*-concat-nested-namespaces)
{
  /// \brief Inline namespace for types and type aliases.
  /// \headerfile floppy/floppy.h
  /// \ingroup aliases
  inline namespace types
  {
    using u8 = uint8_t;                       ///< 8-bit unsigned integer
    using u16 = uint16_t;                     ///< 16-bit unsigned integer
    using u32 = uint32_t;                     ///< 32-bit unsigned integer
    using u64 = uint64_t;                     ///< 64-bit unsigned integer
    using i8 = int8_t;                        ///< 8-bit signed integer
    using i16 = int16_t;                      ///< 16-bit signed integer
    using i32 = int32_t;                      ///< 32-bit signed integer
    using i64 = int64_t;                      ///< 64-bit signed integer
    using usize = size_t;                     ///< Unsigned integer with pointer size
    using isize = std::make_signed_t<size_t>; ///< Signed integer with pointer size
    using f32 = float;                        ///< Float with 32-bit precision
    using f64 = double;                       ///< Float with 64-bit precision
    using f128 [[maybe_unused]] = long double;///< Float with 128-bit precision

    //NOLINTNEXTLINE(*-runtime-int)
    using ulonglong [[maybe_unused]] = unsigned long long; ///< Alias for <tt>unsigned long long</tt>.

    /// \brief Safe alias for <tt>std::optional</tt>.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    /// \details Defaults to <tt>std::optional</tt> if available, otherwise <tt>std::experimental::optional</tt>.
    /// Example usage with helper functions:
    /// \code {.cpp}
    ///  template <typename T>
    ///  auto first(std::vector<T> const& vec) -> option<T> {
    ///    if(vec.empty())
    ///     return none;
    ///    return some(vec.front());
    ///  }
    /// \endcode
    /// \see some
    /// \see none
    /// \sa https://en.cppreference.com/w/cpp/utility/optional
    template <std::destructible T>
    using option = std::optional<T>;

    /// \brief Types implementation details.
    namespace detail
    {
      using tl::expected;
      using tl::unexpected;
    } // namespace detail

    /// \brief Expected type with default error type (<tt>std::string</tt>).
    /// \details Provides result type, which defaults to <tt>std::expected</tt>, if
    /// available, or <tt>tl::expected</tt>/<tt>std::experimental::expected</tt> otherwise.
    ///
    /// Example usage with void return type:
    /// \code {.cpp}
    /// using namespace fl::types;
    ///
    /// auto returns_void(int x) -> result<> {
    ///   if(x > 0)
    ///     return error("x is positive, should be negative ({})", x);
    ///   return Ok();
    /// }
    /// \endcode
    ///
    /// Example usage with non-void return type:
    /// \code {.cpp}
    /// using namespace fl::types;
    /// auto divide_by_then_sqrt(float a, float x) -> result<f32> {
    ///   if(x == 0)
    ///     return error("division by zero");
    ///   return ok(a / x).map(std::sqrt);
    /// }
    /// \endcode
    template <typename T = void>
    using result = detail::expected<T, std::string>;

    static_assert(std::is_same_v<result<>, detail::expected<void, std::string>>);

    /// \brief Helper function for <tt>result</tt>.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    template<typename... Args>
    [[nodiscard]] auto error(std::string_view format, Args&&... args) -> detail::unexpected<std::decay_t<std::string>> {
      return detail::unexpected<std::decay_t<std::string>>(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
    }

    /// \brief Helper function for <tt>result</tt>.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    template <class T>
    [[nodiscard]] auto ok(T&& t) -> detail::expected<std::decay_t<T>, std::string> {
      return detail::expected<std::decay_t<T>, std::string>(std::forward<T>(t));
    }

    /// \brief Helper function for <tt>result</tt>.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    [[nodiscard]] inline auto ok() -> result<> { return {}; }

    /// \brief Helper function for \ref option.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    /// \see none
    template <std::destructible T>
    auto some(T&& t) -> option<std::decay_t<T>> {
      return option<std::decay_t<T>>(std::forward<T>(t));
    }

    /// \brief Helper function for \ref option.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    /// \see some
    inline constexpr auto none = std::nullopt;

    /// \brief Shortcut for <tt>std::reference_wrapper</tt>.
    /// \headerfile floppy/floppy.h
    /// \ingroup aliases
    template <typename T>
    using ref = std::reference_wrapper<T>;
  } // namespace types

  /// \brief Inline namespace for literal operators.
  /// \headerfile floppy/floppy.h
  /// \ingroup aliases
  inline namespace literals
  {
    //#if !defined(__cpp_size_t_suffix)
      /// \brief Literal operator for <b>signed version</b> of <tt>size_t</tt>/\ref types::isize.
      /// \details Since version <b>1.2.1</b> is always available.
      /// \sa https://en.cppreference.com/w/cpp/language/operator_literal
      constexpr auto operator""_z(ulonglong value) -> isize { return static_cast<isize>(value); }

      /// \brief Literal operator for <b>signed version</b> of <tt>size_t</tt>/\ref types::isize.
      /// \details Since version <b>1.2.1</b> is always available.
      /// \sa https://en.cppreference.com/w/cpp/language/operator_literal
      constexpr auto operator""_Z(ulonglong value) -> isize { return static_cast<isize>(value); }

      /// \brief Literal operator for <tt>size_t</tt>/\ref types::usize.
      /// \details Since version <b>1.2.1</b> is always available.
      /// \sa https://en.cppreference.com/w/cpp/language/operator_literal
      constexpr auto operator""_zu(ulonglong value) -> usize { return static_cast<usize>(value); }

      /// \brief Literal operator for <tt>size_t</tt>/\ref types::usize.
      /// \details Since version <b>1.2.1</b> is always available.
      /// \sa https://en.cppreference.com/w/cpp/language/operator_literal
      constexpr auto operator""_ZU(ulonglong value) -> usize { return static_cast<usize>(value); }
    //#endif // defined(__cpp_size_t_suffix)

    /// \brief Literal operator for \ref types::u8.
    constexpr auto operator""_u8(ulonglong value) -> u8 { return static_cast<u8>(value); }

    /// \brief Literal operator for \ref types::u16.
    constexpr auto operator""_u16(ulonglong value) -> u16 { return static_cast<u16>(value); }

    /// \brief Literal operator for \ref types::u32.
    constexpr auto operator""_u32(ulonglong value) -> u32 { return static_cast<u32>(value); }

    /// \brief Literal operator for \ref types::u64.
    constexpr auto operator""_u64(ulonglong value) -> u64 { return static_cast<u64>(value); }

    /// \brief Literal operator for \ref types::i8.
    constexpr auto operator""_i8(ulonglong value) -> i8 { return static_cast<i8>(value); }

    /// \brief Literal operator for \ref types::i16.
    constexpr auto operator""_i16(ulonglong value) -> i16 { return static_cast<i16>(value); }

    /// \brief Literal operator for \ref types::i32.
    constexpr auto operator""_i32(ulonglong value) -> i32 { return static_cast<i32>(value); }

    /// \brief Literal operator for \ref types::i64.
    constexpr auto operator""_i64(ulonglong value) -> i64 { return static_cast<i64>(value); }

    /// \brief Literal operator for <tt>std::ptrdiff_t</tt>.
    constexpr auto operator""_ptrdiff(ulonglong value) -> std::ptrdiff_t { return static_cast<std::ptrdiff_t>(value); }

    // NOLINTBEGIN(*-pro-type-reinterpret-cast, *-no-int-to-ptr)

    /// \brief Literal operator for <tt>void*</tt>.
    /// \note This operator is not constexpr.
    inline auto operator""_pvoid(unsigned long long value) -> void* { return reinterpret_cast<void*>(value); }

    // NOLINTEND(*-pro-type-reinterpret-cast, *-no-int-to-ptr)
  } // namespace literals
} // namespace floppy

/// \defgroup aliases Aliases
/// \brief Aliases for types.