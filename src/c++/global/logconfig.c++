#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <simkernel/global/logconfig.h>

using std::vector;
using std::shared_ptr;
using std::string_view;
using namespace std::string_view_literals;
using namespace simkernel::types;

constexpr auto DEFAULT_LOGGER_NAME = ""sv; // NOLINT(*-redundant-string-init)
auto NO_REINIT_FLAG = false;

namespace simkernel::log
{
  auto LogFileConfiguration::initialize() -> bool
  {
    using std::make_shared;
    using std::begin;
    using std::end;
    using std::move;

    if(NO_REINIT_FLAG)
      return false;

    vector<spdlog::sink_ptr> sinks;
    sinks.push_back(make_shared<spdlog::sinks::stdout_color_sink_st>());
    sinks.push_back(make_shared<spdlog::sinks::rotating_file_sink_st>(
      string(this->filename),
      this->max_size,
      this->max_files
    ));
    const auto logger = make_shared<spdlog::logger>(DEFAULT_LOGGER_NAME.data(), begin(sinks), end(sinks));

    spdlog::set_default_logger(move(logger));
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%=5!o] %^[%=7!l] thread [%t] %-20!s:%-3!#: %v %$");
    spdlog::flush_on(spdlog::level::debug);

    spdlog::info("-----------------------------------------");
    spdlog::info("session started at timestamp {}", std::time(nullptr));
    spdlog::info("-----------------------------------------");

    NO_REINIT_FLAG = true;
    return true;
  }
}
