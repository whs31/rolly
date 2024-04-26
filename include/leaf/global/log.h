#pragma once

#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif

#include <string>
#include <spdlog/spdlog.h>

namespace leaf::log {
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
} // namespace leaf::log

namespace llog = leaf::log;
