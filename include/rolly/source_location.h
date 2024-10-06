#pragma once

#include "format/ostream_formatter.h"
#include "global/definitions.h"
#include "types/stdint.h"

namespace rolly
{
  /**
   * @brief Provides information about the location of the current source file.
   * @details Class provides information about the location of the current source file.
   * This class is a port of C++20 std::source_location.
   * Example usage:
   * @code {.cpp}
   * auto main() -> int {
   *   log::debug("I am called from {}, {}:{} in file {}!",
   *     rolly::source_location::current().function_name(),
   *     rolly::source_location::current().line(),
   *     rolly::source_location::current().column(),
   *     rolly::source_location::current().file_name()
   *   );
   *   fmt::println("I am called from {}", rolly::source_location::current());
   * }
   * @endcode
   *
   * Output:
   * @code {.sh}
   * I am called from main, 5:7 in file main.c++!
   * I am called from main.c++:main 6:7
   * @endcode
   * @note This implementation can be aliased to `std::source_location`, if the compiler supports it. 
   * @see https://en.cppreference.com/w/cpp/utility/source_location
   */
  struct source_location
    {
     private:
      constexpr static auto unknown = "(unknown)";

     public:
      /**
       * @brief Returns current source location.
       */ 
      static ___constexpr___ source_location current(
  #ifndef ROLLY_DOC
  #if not defined(__apple_build_version__) and defined(__clang__) and (__clang_major__ >= 9)
        char const* file = __builtin_FILE(),
        char const* function = __builtin_FUNCTION(),
        u32 line = __builtin_LINE(),
        u32 column = __builtin_COLUMN()
  #elif defined(__GNUC__) and (__GNUC__ > 4 or (__GNUC__ == 4 and __GNUC_MINOR__ >= 8))
        char const* file = __builtin_FILE(),
        char const* function = __builtin_FUNCTION(),
        u32 line = __builtin_LINE(),
        u32 column = 0
  #else // compiler
        char const* file = UNKNOWN,
        char const* function = UNKNOWN,
        u32 line = 0,
        u32 column = 0
  #endif // compiler
  #endif // ROLLY_DOC
      ) noexcept {
        return { file, line, function, column };
      }

     private:
      std::string file_;
      std::string function_;
      u32 line_;
      u32 column_;

     public:
      /**
       * @brief Creates an invalid source_location.
       */
      ___constexpr___ source_location() noexcept
        : file_(unknown)
        , function_(unknown)
        , line_(0)
        , column_(0)
      {}

      /**
       * @brief Creates a source_location from given parameters.
       * @param file File name
       * @param line Number of the line in the file. 0 if NULL
       * @param function Function name
       * @param column Column number in the function. Defaults to 0
       */
      ___constexpr___ source_location(
        char const* file,
        u32 const line,
        char const* function,
        u32 const column = 0
      ) noexcept
        : file_(file)
        , function_(function)
        , line_(line)
        , column_(column)
      {}

      ~source_location() = default;

      /**
       * @brief Returns the file name.
       */
      [[nodiscard]] ___constexpr___ std::string_view file_name() const noexcept { return this->file_; }

      /**
       * @brief Returns a mutable reference to the file name.
       */
      [[nodiscard]] [[maybe_unused]] ___constexpr___ std::string& file_name_mut() noexcept { return this->file_; }

      /**
       * @brief Returns the function name. NULL if unknown or not available on compiler
       */
      [[nodiscard]] ___constexpr___ std::string_view function_name() const noexcept { return this->function_; }

      /**
       * @brief Returns a mutable reference to the function name.
       */
      [[nodiscard]] [[maybe_unused]] ___constexpr___ std::string& function_name_mut() noexcept { return this->function_; }

      /**
       * @brief Returns the line number. 0 if unknown or not available on compiler.
       */
      [[nodiscard]] constexpr u32 line() const noexcept { return this->line_; }

      /**
       * @brief Returns a mutable reference to the line number.
       */
      [[nodiscard]] [[maybe_unused]] constexpr u32& line_mut() noexcept { return this->line_; }

      /**
       * @brief Returns the column number. 0 if unknown or not available on compiler
       */
      [[nodiscard]] constexpr u32 column() const noexcept { return this->column_; }

      /**
       * @brief Returns a mutable reference to the column number.
       */
      [[nodiscard]] [[maybe_unused]] constexpr u32& column_mut() noexcept { return this->column_; }
    };

    /**
     * @brief Stream operator for source_location
     */
    template <class E, class T>
    std::basic_ostream<E, T>& operator<<(std::basic_ostream<E, T>& os, source_location const& loc)
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

  inline bool operator==(source_location const& lhs, source_location const& rhs) noexcept {
    return lhs.file_name() == rhs.file_name()
      and lhs.line() == rhs.line()
      and lhs.function_name() == rhs.function_name()
      and lhs.column() == rhs.column();
  }

  inline bool operator!=(source_location const& lhs, source_location const& rhs) noexcept {
    return not (lhs == rhs);
  }
} // namespace rolly

/**
 * @brief Formatter for @ref rolly::source_location
 */
template <> struct [[maybe_unused]] fmt::formatter<rolly::source_location> : rolly::ostream_formatter<char> {};