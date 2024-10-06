#include <cstdint>
#include <gtest/gtest.h>
#include <floppy/logging.h>

TEST(Logging, Simple)
{
  namespace log = floppy::log;
  spdlog::set_level(spdlog::level::trace);
  auto str = std::string_view("Floppy");
  log::log(log::level::trace, "Hello, {}!", str);
  log::log(log::level::debug, "Hello, {}!", str);
  log::log(log::level::info, "Hello, {}!", str);
  log::log(log::level::warn, "Hello, {}!", str);
  log::log(log::level::err, "Hello, {}!", str);
  log::error("Hello, {}!", str);
  log::warn("Hello, {}!", str);
  log::info("Hello, {}!", str);
  log::debug("Hello, {}!", str);
  log::trace("Hello, {}!", str);
}