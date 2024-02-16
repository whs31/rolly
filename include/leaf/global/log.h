#pragma once

#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <string>
#include <spdlog/spdlog.h>

//#ifdef DOXYGEN

/**
 * \ingroup macros
 * \brief Выводит форматированное сообщение с уровнем логгирования **Trace**.
 * \details Макрос принимает литерал форматирования и произвольное число аргументов для форматированного вывода.
 * Аргументы форматирования должны иметь спецификацию `fmt::formatter`.
 * Если в активном логгере стоит уровень выше, чем `Trace`, то содержимое макроса не будет выведено.
 * Пример:
 * ```cpp
 * $trace("Hello, {}!", "world");
 * ```
 * Вывод:
 * ```sh
 * Hello, world!
 * ```
 * https://github.com/gabime/spdlog
 * \deprecated Макросы - это плохо. Мы больше не используем макросы. Вместо этого макроса теперь используется функция
 * `simkernel::log::trace`, а для добавления имени функции - `simkernel::source_location`.
 * \see simkernel::log::trace
 */
#define $trace(...) SPDLOG_TRACE(__VA_ARGS__)

/**
 * \ingroup macros
 * \brief Выводит форматированное сообщение с уровнем логгирования **Debug**.
 * \details Макрос принимает литерал форматирования и произвольное число аргументов для форматированного вывода.
 * Аргументы форматирования должны иметь спецификацию `fmt::formatter`.
 * Если в активном логгере стоит уровень выше, чем `Debug`, то содержимое макроса не будет выведено.
 * Пример:
 * ```cpp
 * $debug("Hello, {}!", "world");
 * ```
 * Вывод:
 * ```sh
 * Hello, world!
 * ```
 * https://github.com/gabime/spdlog
 * \deprecated Макросы - это плохо. Мы больше не используем макросы. Вместо этого макроса теперь используется функция
 * `simkernel::log::debug`, а для добавления имени функции - `simkernel::source_location`.
 * \see simkernel::log::debug
 */
#define $debug(...) SPDLOG_DEBUG(__VA_ARGS__)

/**
 * \ingroup macros
 * \brief Выводит форматированное сообщение с уровнем логгирования **Info**.
 * \details Макрос принимает литерал форматирования и произвольное число аргументов для форматированного вывода.
 * Аргументы форматирования должны иметь спецификацию `fmt::formatter`.
 * Если в активном логгере стоит уровень выше, чем `Info`, то содержимое макроса не будет выведено.
 * Пример:
 * ```cpp
 * $info("Hello, {}!", "world");
 * ```
 * Вывод:
 * ```sh
 * Hello, world!
 * ```
 * https://github.com/gabime/spdlog
 * \deprecated Макросы - это плохо. Мы больше не используем макросы. Вместо этого макроса теперь используется функция
 * `simkernel::log::info`, а для добавления имени функции - `simkernel::source_location`.
 * \see simkernel::log::info
 */
#define $info(...) SPDLOG_INFO(__VA_ARGS__)

/**
 * \ingroup macros
 * \brief Выводит форматированное сообщение с уровнем логгирования **Warning**.
 * \details Макрос принимает литерал форматирования и произвольное число аргументов для форматированного вывода.
 * Аргументы форматирования должны иметь спецификацию `fmt::formatter`.
 * Если в активном логгере стоит уровень выше, чем `Warning`, то содержимое макроса не будет выведено.
 * Пример:
 * ```cpp
 * $warn("Hello, {}!", "world");
 * ```
 * Вывод:
 * ```sh
 * Hello, world!
 * ```
 * https://github.com/gabime/spdlog
 * \deprecated Макросы - это плохо. Мы больше не используем макросы. Вместо этого макроса теперь используется функция
 * `simkernel::log::warn`, а для добавления имени функции - `simkernel::source_location`.
 * \see simkernel::log::warn
 */
#define $warn(...) SPDLOG_WARN(__VA_ARGS__)

/**
 * \ingroup macros
 * \brief Выводит форматированное сообщение с уровнем логгирования **Critical**.
 * \details Макрос принимает литерал форматирования и произвольное число аргументов для форматированного вывода.
 * Аргументы форматирования должны иметь спецификацию `fmt::formatter`.
 * Если в активном логгере стоит уровень выше, чем `Critical`, то содержимое макроса не будет выведено.
 * Пример:
 * ```cpp
 * $critical("Hello, {}!", "world");
 * ```
 * Вывод:
 * ```sh
 * Hello, world!
 * ```
 * https://github.com/gabime/spdlog
 * \deprecated Макросы - это плохо. Мы больше не используем макросы. Вместо этого макроса теперь используется функция
 * `simkernel::log::critical`, а для добавления имени функции - `simkernel::source_location`.
 * \see simkernel::log::critical
 */
#define $critical(...) SPDLOG_CRITICAL(__VA_ARGS__)

/**
 * \ingroup macros
 * \brief Выводит форматированное сообщение с уровнем логгирования **Error**.
 * \details Макрос принимает литерал форматирования и произвольное число аргументов для форматированного вывода.
 * Аргументы форматирования должны иметь спецификацию `fmt::formatter`.
 * Пример:
 * ```cpp
 * $error("Hello, {}!", "world");
 * ```
 * Вывод:
 * ```sh
 * Hello, world!
 * ```
 * https://github.com/gabime/spdlog
 * \deprecated Макросы - это плохо. Мы больше не используем макросы. Вместо этого макроса теперь используется функция
 * `simkernel::log::error`, а для добавления имени функции - `simkernel::source_location`.
 * \see simkernel::log::error
 */
#define $error(...) SPDLOG_ERROR(__VA_ARGS__)

//#endif

namespace leaf::log
{
  using spdlog::trace;
  using spdlog::debug;
  using spdlog::info;
  using spdlog::warn;
  using spdlog::error;
  using spdlog::critical;

  using namespace std::string_view_literals;

  constexpr auto line_up = "\033[A"sv;
  constexpr auto line_clear = "\033[2K"sv;
  constexpr auto rewrite = "\033[A\033[2K\r"sv;
  constexpr auto clean = "\033[2K\r"sv;
}

namespace llog = leaf::log;
