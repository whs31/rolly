#pragma once

#include <string_view>
#include <sstream>
#include <iosfwd>
#include <fmt/format.h>

namespace rolly {
  /**
   * @brief Formatter for types which support stream output.
   * @details Example usage for <tt>std::ostream</tt>:
   * @code {.cpp}
   * template <> struct [[maybe_unused]] fmt::formatter<YourType> : rolly::ostream_formatter<char>
   * {};
   * @endcode
   *
   * Example usage for <tt>std::wostream</tt>:
   * @code {.cpp}
   * template <> struct [[maybe_unused]] fmt::formatter<YourType> :
   * rolly::ostream_formatter<wchar_t> {};
   * @endcode
   * @tparam C Underlying character type.
   */
  template <typename C>
  struct [[maybe_unused]] ostream_formatter : fmt::formatter<std::basic_string_view<C>, C> {
    /**
     * @brief Required by <tt>fmt</tt>.
     */
    template <typename T, typename OutputIt>
    OutputIt format(T const& value, fmt::basic_format_context<OutputIt, C>& ctx) const {
      auto ss = std::basic_stringstream<C>();
      ss << value;
      return fmt::formatter<std::basic_string_view<C>, C>::format(ss.str(), ctx);
    }
  };
}  // namespace rolly
