#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

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
 * \see https://github.com/gabime/spdlog
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
 * \see https://github.com/gabime/spdlog
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
 * \see https://github.com/gabime/spdlog
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
 * \see https://github.com/gabime/spdlog
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
 * \see https://github.com/gabime/spdlog
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
 * \see https://github.com/gabime/spdlog
 */
#define $error(...) SPDLOG_ERROR(__VA_ARGS__)