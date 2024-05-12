#pragma once

#include <lf/detail/export.h>
#include <lf/detail/backports.h>
#include <lf/detail/log.h>

namespace lf
{
  /**
   * \brief Terminates program execution with detailed error message.
   * \details Logs the error message and terminates program execution via <tt>std::terminate</tt>
   * \note You can catch the panic via <tt>std::set_terminate</tt> custom handler, but it's not recommended.
   * \sa https://en.cppreference.com/w/cpp/error/terminate
   * \sa https://en.cppreference.com/w/cpp/error/terminate_handler
   * \param message Error message.
   * \param location Source location of the panic. Defaults to current location.
   */
  [[noreturn]] inline auto panic(std::string_view message, source_location location = source_location::current()) -> void
  {
    log::critical("panic in file `{}` line {}", location.file_name(), location.line());
    log::critical("in function `{}`", location.function_name());
    log::critical("reason: {}", message);
    log::critical("terminate will be called now.");
    std::terminate();
  }
}