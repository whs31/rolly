#pragma once

#include <cstdint>
#include <ranges>
#include <fmt/format.h>
#include <floppy/detail/platform.h>
#include <floppy/detail/concepts.h>
#include <floppy/detail/formatters.h>

#if defined(__cpp_lib_source_location)
# include <source_location>
#endif // __cpp_lib_source_location

namespace floppy
{
  /// \brief <tt>std::ranges</tt> backports namespace.
  /// \details Includes full <tt>std::ranges</tt> namespace.
  namespace ranges
  {
    using namespace std::ranges; // NOLINT(*-build-using-namespace)

    /// \brief Implementation details for the ranges backports.
    namespace detail
    {
      /// \brief Converts a source range to a non-view object.
      template <typename C> struct to_helper {};

      /// \brief Propagates conversion to a non-view object.
      template <typename C, range R>
        requires std::convertible_to<range_value_t<R>, typename C::value_type>
      auto operator|(R&& r, [[maybe_unused]] to_helper<C> _) -> C // NOLINT(*-missing-std-forward)
      {
        return C { r.begin(), r.end() };
      }
    } // namespace detail

    /// \brief The overloads of the range conversion function construct a new non-view object from a source range.
    /// \headerfile floppy/floppy.h
    /// \ingroup backports
    /// \details This is an alias to <tt>std::ranges::to</tt>.
    /// Example usage:
    /// \code {.cpp}
    /// auto main() -> int {
    ///   auto vec = std::views::iota(1, 5)
    ///     | std::views::transform([](int i) { return i * 2; })
    ///     | fl::ranges::collect<std::vector>();
    ///   return 0;
    /// }
    /// \endcode
    /// \sa https://en.cppreference.com/w/cpp/ranges/to
    /// \see to
    template <range C> requires (not view<C>)
    auto collect() { return detail::to_helper<C> {}; }

    /// \brief The overloads of the range conversion function construct a new non-view object from a source range.
    /// \headerfile floppy/floppy.h
    /// \ingroup backports
    /// \sa https://en.cppreference.com/w/cpp/ranges/to
    /// \see collect
    template <range C> requires (not view<C>)
    auto to() { return detail::to_helper<C> {}; }
  } // namespace ranges

  /// \brief Invokes undefined behavior.
  /// \headerfile floppy/floppy.h
  /// \ingroup backports
  /// \details This is backport of C++23 <tt>std::unreachable</tt>.
  /// Mostly used for silence compiler warnings.
  /// Example usage:
  /// \code {.cpp}
  /// enum class color { red, green, blue };
  /// auto to_string(color c) -> std::string {
  ///   switch (c) {
  ///     case color::red: return "red";
  ///     case color::green: return "green";
  ///     case color::blue: return "blue";
  ///     default: fl::unreachable();
  ///   }
  /// }
  /// \endcode
  /// \sa https://en.cppreference.com/w/cpp/utility/unreachable
  [[noreturn]] inline auto unreachable() -> void {
    #if defined(FLOPPY_COMPILER_MSVC) && !defined(FLOPPY_COMPILER_CLANG)
        __assume(false);
    #else
        __builtin_unreachable();
    #endif
  }

  /// \brief Converts enum type to its underlying type.
  /// \headerfile floppy/floppy.h
  /// \ingroup backports
  /// \details This is backport of C++20 (23) <tt>std::to_underlying</tt>.
  /// Example usage:
  /// \code {.cpp}
  /// enum class A : char { val };
  /// enum class B : int { val };
  /// enum class C : short { val };
  /// auto main() -> int {
  ///   fmt::print("a underlying type: {}, value: {}\n", fl::rtti::type_name<decltype(fl::to_underlying(A::val))>(), fl::to_underlying(A::val));
  ///   fmt::print("b underlying type: {}, value: {}\n", fl::rtti::type_name<decltype(fl::to_underlying(B::val))>(), fl::to_underlying(B::val));
  ///   fmt::print("c underlying type: {}, value: {}\n", fl::rtti::type_name<decltype(fl::to_underlying(C::val))>(), fl::to_underlying(C::val));
  ///   return 0;
  /// }
  /// \endcode
  ///
  /// Output:
  /// \code {.sh}
  /// a underlying type: char, value: 0
  /// b underlying type: int, value: 0
  /// c underlying type: short, value: 0
  /// \endcode
  /// \tparam T Enum type. Must be an enum type.
  /// \param t Enum value
  /// \return Underlying value of the enum
  /// \sa https://en.cppreference.com/w/cpp/utility/to_underlying
  template <concepts::enum_ T>
  constexpr auto to_underlying(T t) noexcept -> std::underlying_type_t<T> {
    return static_cast<std::underlying_type_t<T>>(t);
  }

  /// \brief Provides information about the location of the current source file.
  /// \headerfile floppy/floppy.h
  /// \ingroup backports
  /// \details Class provides information about the location of the current source file.
  /// This class is a port of C++20 std::source_location.
  /// Example usage:
  /// \code {.cpp}
  /// auto main() -> int {
  ///   log::debug("I am called from {}, {}:{} in file {}!",
  ///     lf::source_location::current().function_name(),
  ///     lf::source_location::current().line(),
  ///     lf::source_location::current().column(),
  ///     lf::source_location::current().file_name()
  ///   );
  ///   log::debug("I am called from {}", lf::source_location::current());
  /// }
  /// \endcode
  ///
  /// Output:
  /// \code {.sh}
  /// I am called from main, 5:7 in file main.c++!
  /// I am called from main.c++:main 6:7
  /// \endcode
  /// \note This implementation can be aliased to `std::source_location`, if the compiler supports it.
  /// Otherwise, it is a self-contained port of C++20 std::source_location.
  #if defined(__cpp_lib_source_location)
    using source_location = std::source_location;
  #else // __cpp_lib_source_location
    struct source_location
    {
     private:
      constexpr static auto UNKNOWN = "(unknown)";

     public:
      /// \brief Returns current source location.
      /// \note Returns actual current source location only if called with default arguments.
  #if not defined(__apple_build_version__) and defined(__clang__) and (__clang_major__ >= 9)
      static consteval auto current(
        char const* file = __builtin_FILE(),
        char const* function = __builtin_FUNCTION(),
        uint_least32_t line = __builtin_LINE(),
        uint_least32_t column = __builtin_COLUMN()
          ) noexcept -> source_location
  #elif defined(__GNUC__) and (__GNUC__ > 4 or (__GNUC__ == 4 and __GNUC_MINOR__ >= 8))
      static consteval auto current(
        char const* file = __builtin_FILE(),
        char const* function = __builtin_FUNCTION(),
        uint_least32_t line = __builtin_LINE(),
        uint_least32_t column = 0
        ) noexcept -> source_location
  #else // compiler
      static consteval auto current(
        char const* file = UNKNOWN,
        char const* function = UNKNOWN,
        uint_least32_t line = 0,
        uint_least32_t column = 0
        ) noexcept -> source_location
  #endif // compiler
      {
        return { file, line, function, column };
      }

     private:
      char const* m_file;
      char const* m_function;
      uint_least32_t m_line;
      uint_least32_t m_column;

     public:

      /// \brief Creates an empty invalid source_location.
      constexpr source_location() noexcept
        : m_file(UNKNOWN)
        , m_function(UNKNOWN)
        , m_line(0)
        , m_column(0)
      {}

      /// \brief Creates a source_location from given parameters.
      /// \param file File name
      /// \param line Number of the line in the file. 0 if NULL
      /// \param function Function name
      /// \param column Column number in the function. Defaults to 0
      constexpr source_location(
        char const* file,
        uint_least32_t const line,
        char const* function,
        uint_least32_t const column = 0
      ) noexcept
        : m_file(file)
        , m_function(function)
        , m_line(line)
        , m_column(column)
      {}

      /// \brief Returns the file name.
      [[nodiscard]] constexpr auto file_name() const noexcept -> char const* { return this->m_file; }

      /// \brief Returns the function name. NULL if unknown or not available on compiler
      [[nodiscard]] constexpr auto function_name() const noexcept -> char const* { return this->m_function; }

      /// \brief Returns the line number. 0 if unknown or not available on compiler
      [[nodiscard]] constexpr auto line() const noexcept -> uint_least32_t { return this->m_line; }

      /// \brief Returns the column number. 0 if unknown or not available on compiler
      [[nodiscard]] constexpr auto column() const noexcept -> uint_least32_t { return this->m_column; }
    };

    /// \brief Stream adaptor for source_location
    template <class E, class T>
    auto operator<<(std::basic_ostream<E, T>& os, source_location const& loc) -> std::basic_ostream<E, T>&
    {
      os.width(0);
      if(loc.line() == 0)
        os << "(unknown)";
      else {
        os << fmt::format("{}:{}", loc.file_name(), loc.line());
        if(loc.column())
          os << fmt::format(":{}", loc.column());
        os << fmt::format(": in fn {} ", loc.function_name());
      }
      return os;
    }
  #endif // __cpp_lib_source_location
} // namespace floppy

/// \brief Formatter for `floppy::source_location`
template <> struct [[maybe_unused]] fmt::formatter<floppy::source_location> : floppy::ostream_formatter<char> {};

/// \brief Formatter for `floppy::meta::version`
template <> struct [[maybe_unused]] fmt::formatter<floppy::meta::version> : floppy::ostream_formatter<char> {};

/// \brief Formatter for `floppy::meta::project_meta`
template <> struct [[maybe_unused]] fmt::formatter<floppy::meta::project_meta> : floppy::ostream_formatter<char> {};

/// \defgroup backports Backports
/// \brief Backports of newer features for older compilers.