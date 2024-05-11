#pragma once

#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif

#include <string>
#include <spdlog/spdlog.h>

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

  [[nodiscard]] inline auto default_logger() -> std::shared_ptr<spdlog::logger> {
    return spdlog::default_logger();
  }

  template <typename... Args>
  auto trace(spdlog::logger* logger, std::string_view format, Args&&... args) -> void {
    logger->trace(format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto debug(spdlog::logger* logger, std::string_view format, Args&&... args) -> void {
    logger->debug(format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto info(spdlog::logger* logger, std::string_view format, Args&&... args) -> void {
    logger->info(format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto warn(spdlog::logger* logger, std::string_view format, Args&&... args) -> void {
    logger->warn(format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto error(spdlog::logger* logger, std::string_view format, Args&&... args) -> void {
    logger->error(format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto critical(spdlog::logger* logger, std::string_view format, Args&&... args) -> void {
    logger->critical(format, std::forward<Args>(args)...);
  }
} // namespace leaf::log

namespace llog = leaf::log;
