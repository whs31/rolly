#pragma once

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <rll/global/definitions.h>
#include <rll/stdint.h>

#if defined(_MSC_VER)
#  include <ciso646>
#endif

namespace rll {
  /**
   * @brief Provides information about the location of the current source file.
   * @details Class provides information about the location of the current source file.
   * This class is a port of C++20 std::source_location.
   * Example usage:
   * @code {.cpp}
   * auto main() -> int {
   *   log::debug("I am called from {}, {}:{} in file {}!",
   *     rll::source_location::current().function_name(),
   *     rll::source_location::current().line(),
   *     rll::source_location::current().column(),
   *     rll::source_location::current().file_name()
   *   );
   *   fmt::println("I am called from {}", rll::source_location::current());
   * }
   * @endcode
   *
   * Output:
   * @code {.sh}
   * I am called from main, 5:7 in file main.c++!
   * I am called from main.c++:main 6:7
   * @endcode
   * @note This implementation can be aliased to `std::source_location`, if the compiler supports
   * it.
   * @see https://en.cppreference.com/w/cpp/utility/source_location
   */
  struct source_location {
   private:
    constexpr static auto unknown = "(unknown)";

   public:
    /**
     * @brief Returns current source location.
     */
    static constexpr source_location current(
#ifndef RLL_DOC
#  if not defined(__apple_build_version__) and defined(__clang__) and (__clang_major__ >= 9)
      char const* file = __builtin_FILE(),
      char const* function = __builtin_FUNCTION(),
      u32 line = __builtin_LINE(),
      u32 column = __builtin_COLUMN()
#  else
      char const* file = __builtin_FILE(),
      char const* function = __builtin_FUNCTION(),
      u32 line = __builtin_LINE(),
      u32 column = 0
#  endif  // compiler
#endif    // RLL_DOC
    ) noexcept {
      return {file, line, function, column};
    }

   private:
    char const* file_;
    char const* function_;
    u32 line_;
    u32 column_;

   public:
    /**
     * @brief Creates an invalid source_location.
     */
    constexpr source_location() noexcept
      : file_(unknown)
      , function_(unknown)
      , line_(0)
      , column_(0) {}

    /**
     * @brief Creates a source_location from given parameters.
     * @param file File name
     * @param line Number of the line in the file. 0 if NULL
     * @param function Function name
     * @param column Column number in the function. Defaults to 0
     */
    constexpr source_location(
      char const* file,
      u32 const line,
      char const* function,
      u32 const column = 0
    ) noexcept
      : file_(file)
      , function_(function)
      , line_(line)
      , column_(column) {}

    ~source_location() = default;

    /**
     * @brief Returns the file name.
     */
    [[nodiscard]] constexpr std::string_view file_name() const noexcept { return this->file_; }

    /**
     * @brief Returns the function name. NULL if unknown or not available on compiler
     */
    [[nodiscard]] constexpr std::string_view function_name() const noexcept {
      return this->function_;
    }

    /**
     * @brief Returns the line number. 0 if unknown or not available on compiler.
     */
    [[nodiscard]] constexpr u32 line() const noexcept { return this->line_; }

    /**
     * @brief Returns the column number. 0 if unknown or not available on compiler
     */
    [[nodiscard]] constexpr u32 column() const noexcept { return this->column_; }
  };

  /**
   * @brief Stream operator for source_location
   */
  template <class E, class T>
  std::basic_ostream<E, T>& operator<<(std::basic_ostream<E, T>& os, source_location const& loc) {
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

  inline bool operator==(source_location const& lhs, source_location const& rhs) noexcept {
    return lhs.file_name() == rhs.file_name() and lhs.line() == rhs.line()
       and lhs.function_name() == rhs.function_name() and lhs.column() == rhs.column();
  }

  inline bool operator!=(source_location const& lhs, source_location const& rhs) noexcept {
    return not (lhs == rhs);
  }
}  // namespace rll

/**
 * @brief Specialization of the `fmt::formatter` for the @ref rll::source_location
 * class.
 * @relates rll::source_location
 */
template <>
struct [[maybe_unused]] fmt::formatter<rll::source_location> : ostream_formatter {};
