#pragma once

#include <fmt/format.h>
#include <fmt/color.h>

/// \brief Print helpers namespace.
namespace floppy::print_helpers
{
  /// \brief Print helpers internal namespace.
  namespace detail
  {
    /// \brief Prints a message to the console with google test style.
    /// \param format The format string.
    /// \param args The format arguments.
    /// \tparam Args The format arguments types (inferred from the format string and args).
    template <typename... Args>
    auto google_test_print(fmt::format_string<Args...> format, Args&&... args) -> void {
      fmt::println("\u001b[32m[          ] \u001b[33m{}\u001b[0m", fmt::format(format, std::forward<Args>(args)...));
    }
  } // namespace detail

  /// \brief Prints a message to stderr with error formatting.
  /// \headerfile floppy/floppy.h
  /// \ingroup helpers
  /// \param format The format string.
  /// \param args The format arguments.
  /// \tparam Args The format arguments types (inferred from the format string and args).
  template <typename... Args>
  auto critical_message(std::string_view format, Args&&... args) -> void {
    fmt::print(
      stderr,
      fmt::emphasis::bold | fg(fmt::color::red),
      "fatal: {}\n",
      fmt::format(fmt::runtime(format), std::forward<Args>(args)...)
    );
  }
} // namespace floppy::print_helpers

/// \defgroup macros Macros
/// \brief Macros defined in public headers of the library.

/// \brief Prints a message to the console with google test style.
/// \headerfile floppy/floppy.h
/// \ingroup helpers
/// \details Use this function to print messages in google test style.
/// Example:
/// \code {.cpp}
/// GTEST_PRINT("Hello, {}!", "World");
/// GTEST_PRINT("Hello, {}!", 42);
/// \endcode
/// Output:
/// \code
/// [          ] Hello, World!
/// [          ] Hello, 42!
/// \endcode
/// \note Since version <b>1.2.2</b>, this macro is replaced with template constexpr function. This is
/// the reason it has all upper case name.
template <typename... Args>
[[maybe_unused]] constexpr auto GTEST_PRINT(fmt::format_string<Args...> format, Args&&... args) -> void { // NOLINT(*-identifier-naming)
  ::floppy::print_helpers::detail::google_test_print(format, std::forward<Args>(args)...);
}