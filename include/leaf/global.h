#pragma once

#include <leaf/global/definitions.h>
#include <leaf/global/log.h>
#include <leaf/global/types.h>
#include <leaf/global/expected.h>
#include <leaf/global/sourcelocation.h>

/// \brief Альтернативный алиас на пространство имен `leaf`.
namespace lf = leaf;

namespace leaf
{
  [[noreturn]] inline auto panic(std::string_view message) -> void
  {
    log::critical("panic in file `{}` line {}", lf::source_location::current().file_name(), lf::source_location::current().line());
    log::critical("in function `{}`", lf::source_location::current().function_name());
    log::critical("reason: {}", message);
    log::critical("terminate will be called now.");
    std::terminate();
  }
}