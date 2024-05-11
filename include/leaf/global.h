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
  [[noreturn]] inline auto panic(std::string_view message, source_location location = source_location::current()) -> void
  {
    log::critical("panic in file `{}` line {}", location.file_name(), location.line());
    log::critical("in function `{}`", location.function_name());
    log::critical("reason: {}", message);
    log::critical("terminate will be called now.");
    std::terminate();
  }
}