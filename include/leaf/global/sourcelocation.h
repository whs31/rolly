#pragma once

#include <cstdint>
#include <iosfwd>
#include <fmt/format.h>
#include <leaf/global/types.h>

namespace leaf
{
  /**
   * \brief Предоставляет информацию о месте вызова функции
   * \details Класс представляет из себя портированную версию C++20 std::source_location.
   * Пример использования:
   * ```cpp
   * int main() {
       llog::debug("I am called from {}, {}:{} in file {}!",
         leaf::current_source_location.function_name(),
         leaf::current_source_location.line(),
         leaf::current_source_location.column(),
         leaf::current_source_location.file_name()
       );
       llog::debug("I am called from {}", leaf::current_source_location);
     }
   * ```
   *
   * Вывод:
   * ```sh
   * I am called from main, 5:7 in file main.c++!
   * I am called from main.c++:main 6:7
   * ```
   * \version 3.0.0
   */
  struct source_location
  {
    private:
      constexpr static auto UNKNOWN = "(unknown)";

      const char* m_file;
      const char* m_function;
      uint_least32_t m_line;
      uint_least32_t m_column;

    public:
      /**
       * \brief Создает невалидный объект source_location
       */
      constexpr source_location() noexcept
        : m_file(UNKNOWN),
          m_function(UNKNOWN),
          m_line(0),
          m_column(0)
      {}

      /**
       * \brief Создает объект source_location
       * \param file Имя файла
       * \param line Номер строки
       * \param function Имя функции
       * \param column Номер столбца
       */
      constexpr source_location(const char* file, const uint_least32_t line, const char* function, const uint_least32_t column = 0) noexcept
        : m_file(file),
          m_function(function),
          m_line(line),
          m_column(column)
      {}

      /// \brief Имя файла
      [[nodiscard]] constexpr auto file_name() const noexcept -> const char* { return this->m_file; }

      /// \brief Имя функции, включая пространство имен
      [[nodiscard]] constexpr auto function_name() const noexcept -> const char* { return this->m_function; }

      /// \brief Номер строки
      [[nodiscard]] constexpr auto line() const noexcept { return this->m_line; }

      /// \brief Номер столбца. Если компилятор не поддерживает столбцы, возвращает 0
      [[nodiscard]] constexpr auto column() const noexcept { return this->m_column; }
  };

  /**
   * \ingroup macros
   * \brief Предоставляет информацию о точке, в которой вызвана функция
   */
#define current_source_location source_location(__builtin_FILE(), __builtin_LINE(), __builtin_FUNCTION())

  /// \brief Адаптер std::ostream для source_location
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

/// \brief Адаптер fmt::formatter для source_location
template<>
struct fmt::formatter<leaf::source_location> : fmt::formatter<std::string_view>
{
  template <typename FormatContext>
  auto format(const leaf::source_location& loc, FormatContext &ctx) const
  {
    return fmt::format_to(ctx.out(), "{}:{} {}:{}", loc.file_name(), loc.function_name(), loc.line(), loc.column());
  }
};