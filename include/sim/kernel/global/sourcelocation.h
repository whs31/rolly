#pragma once

#include <cstdint>
#include <iosfwd>
#include <fmt/format.h>
#include <sim/kernel/global/types.h>

namespace sim
{
  struct source_location
  {
    private:
    constexpr static auto UNKNOWN = "(unknown)";

    const char* m_file;
    const char* m_function;
    uint_least32_t m_line;
    uint_least32_t m_column;

    public:
    constexpr source_location() noexcept
      : m_file(UNKNOWN),
        m_function(UNKNOWN),
        m_line(0),
        m_column(0)
    {}

    constexpr source_location(const char* file, const uint_least32_t line, const char* function, const uint_least32_t column = 0) noexcept
      : m_file(file),
        m_function(function),
        m_line(line),
        m_column(column)
    {}

    constexpr auto file_name() const noexcept -> const char* { return this->m_file; }
    constexpr auto function_name() const noexcept -> const char* { return this->m_function; }
    constexpr auto line() const noexcept { return this->m_line; }
    constexpr auto column() const noexcept { return this->m_column; }
  };

  #define current_source_location source_location(__builtin_FILE(), __builtin_LINE(), __builtin_FUNCTION())
  // [[nodiscard]] constexpr auto current_source_location() noexcept -> source_location {
  //   return source_location(__builtin_FILE(), __builtin_LINE(), __builtin_FUNCTION());
  // }

  template<class E, class T>
  auto operator<<(std::basic_ostream<E, T> &os, const source_location &loc) -> std::basic_ostream<E, T>&
  {
    os.width(0);
    if(loc.line() == 0)
      os << "(unknown)";
    else
    {
      os << fmt::format("{}:{}", loc.file_name(), loc.line());
      if(loc.column())
        os << fmt::format(":{}", loc.column());
      os << fmt::format(": in function {} ", loc.function_name());
    }
    return os;
  }
}