#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <fmt/format.h>

#if defined(__cpp_lib_expected) and (__cpp_lib_expected >= 202211L)
# if __has_include(<expected>)
#   include <expected>
# elif __has_include(<experimental/expected>)
#   include <experimental/expected>
# else
#   include <tl/expected.hpp>
# endif
#else
# include <tl/expected.hpp>
#endif

namespace lf // NOLINT(*-concat-nested-namespaces)
{
  /// \brief Inline namespace for types and type aliases.
  inline namespace types
  {
    using u8 = uint8_t;       ///< 8-bit unsigned integer
    using u16 = uint16_t;     ///< 16-bit unsigned integer
    using u32 = uint32_t;     ///< 32-bit unsigned integer
    using u64 = uint64_t;     ///< 64-bit unsigned integer
    using i8 = int8_t;        ///< 8-bit signed integer
    using i16 = int16_t;      ///< 16-bit signed integer
    using i32 = int32_t;      ///< 32-bit signed integer
    using i64 = int64_t;      ///< 64-bit signed integer
    using usize = size_t;     ///< Unsigned integer with pointer size
    using isize = intptr_t;   ///< Signed integer with pointer size
    using f32 = float;        ///< Float with 32-bit precision
    using f64 = double;       ///< Float with 64-bit precision
    using f128 = long double; ///< Float with 128-bit precision

    /**
     * \brief Safe alias for <tt>std::optional</tt>.
     * \details Defaults to <tt>std::optional</tt> if available, otherwise <tt>std::experimental::optional</tt>.
     * Example usage with helper functions:
     * \code {{.cpp}
        template <typename T>
        auto first(std::vector<T> const& vec) -> Option<T> {
          if(vec.empty())
           return None;
          return Some(vec.front());
        }
     * \endcode
     * \sa https://en.cppreference.com/w/cpp/utility/optional
     */
    template <std::destructible T>
    using Option = std::optional<T>;

    /// \brief Helper function for <tt>Option</tt>.
    template <std::destructible T>
    auto Some(T&& t) -> Option<std::decay_t<T>> {
      return Option<std::decay_t<T>>(std::forward<T>(t));
    }

    /// \brief Helper function for <tt>Option</tt>.
    inline constexpr auto None = std::nullopt;

    namespace detail
    {
      #if defined(__cpp_lib_expected) and (__cpp_lib_expected >= 202211L)
      using std::expected;
      using std::unexpected;
      #else
      using tl::expected;
      using tl::unexpected;
      #endif
    }

    /// \brief Namespace for more flexible extensions of provided types.
    namespace flex
    {
      /// \brief Expected type with custom error type.
      template <typename T, typename E>
      using Result = detail::expected<T, E>;

      /// \brief Helper function for <tt>Result<T, E></tt>.
      template <typename E>
      [[nodiscard]] auto Error(E&& e) -> detail::unexpected<std::decay_t<E>> {
        return detail::unexpected<std::decay_t<E>>(std::forward<E>(e));
      }
    }

    /**
     * \brief Expected type with default error type (<tt>std::string</tt>).
     * \details Provides result type, which defaults to <tt>std::expected</tt>, if
     * available, or <tt>tl::expected</tt>/<tt>std::experimental::expected</tt> otherwise.
     *
     * Example usage with void return type:
     * \code {.cpp}
      using namespace lf::types;

      auto returns_void(int x) -> Result<> {
        if(x > 0)
          return Error("x is positive, should be negative ({})", x);
        return Ok();
      }
     * \endcode
     *
     * Example usage with non-void return type:
     * \code {.cpp}
      using namespace lf::types;
      auto divide_by_then_sqrt(float a, float x) -> Result<float> {
        if(x == 0)
          return Error("division by zero");
        return Ok(a / x).map(std::sqrt);
      }
     */
    template <typename T = void>
    using Result = detail::expected<T, std::string>;

    static_assert(std::is_same_v<Result<>, detail::expected<void, std::string>>);

    /// \brief Helper function for <tt>Result</tt>.
    template<typename... Args>
    [[nodiscard]] auto Error(std::string_view format, Args&&... args) -> detail::unexpected<std::decay_t<std::string>> {
      return detail::unexpected<std::decay_t<std::string>>(fmt::format(fmt::runtime(format), std::forward<Args>(args)...));
    }

    /// \brief Helper function for <tt>Result</tt>.
    template <class T>
    [[nodiscard]] auto Ok(T&& t) -> detail::expected<std::decay_t<T>, std::string> {
      return detail::expected<std::decay_t<T>, std::string>(std::forward<T>(t));
    }

    /// \brief Helper function for <tt>Result</tt>.
    [[nodiscard]] inline auto Ok() -> Result<> { return {}; }
  }

  /// \brief Inline namespace for literal operators.
  inline namespace literals
  {
    inline auto operator"" _u8(unsigned long long int value) { return u8(value); }
    inline auto operator"" _u16(unsigned long long int value) { return u16(value); }
    inline auto operator"" _u32(unsigned long long int value) { return u32(value); }
    inline auto operator"" _u64(unsigned long long int value) { return u64(value); }
    inline auto operator"" _i8(unsigned long long int value) { return i8(value); }
    inline auto operator"" _i16(unsigned long long int value) { return i16(value); }
    inline auto operator"" _i32(unsigned long long int value) { return i32(value); }
    inline auto operator"" _i64(unsigned long long int value) { return i64(value); }
    inline auto operator"" _sz(unsigned long long int value) { return usize(value); }
    inline auto operator"" _iptr(unsigned long long int value) { return isize(value); }
    inline auto operator"" _uptr(unsigned long long int value) { return usize(value); }
    inline auto operator"" _ptrdiff(unsigned long long int value) { return ptrdiff_t(value); }
  }
} // namespace leaf::inline types
