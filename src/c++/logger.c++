#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <magic_enum.hpp>
#include <leaf/logger.h>
#include <leaf/conversion.h>
#include <leaf/utils/enum.h>

using std::string;
using std::vector;
using std::shared_ptr;
using std::string_view;
using std::make_shared;
using std::begin;
using std::end;
using std::move;
using namespace std::chrono_literals;
using namespace leaf::types;

namespace leaf
{
  Logger::Logger(
    string_view logger_name, const string_view log_pattern,
    Level level, const Target target,
    optional<string_view> log_file_name,
    optional<types::f32> max_file_size_mb,
    optional<types::usize> max_file_count
  )
    : initialized(false)
  {
    if(this->initialized)
    {
      llog::warn("logger {} already initialized", logger_name);
      return;
    }

    if(log_pattern.empty()) {
      llog::warn("no log pattern provided to logger {}", logger_name);
      return;
    }

    vector<spdlog::sink_ptr> sinks;
    auto bitmask = utils::to_underlying(target);
    auto mask = 1;
    while(bitmask)
    {
      switch(bitmask bitand mask)
      {
        case utils::to_underlying(Target::File):
          if(not log_file_name.has_value()) {
            llog::error("no log file name provided to logger {}", logger_name);
            return;
          }
          if(not max_file_size_mb.has_value()) {
            llog::error("no max file size provided to logger {}", logger_name);
            return;
          }
          if(not max_file_count.has_value()) {
            llog::error("no max file count provided to logger {}", logger_name);
            return;
          }
          sinks.push_back(
            make_shared<spdlog::sinks::rotating_file_sink_mt>(
              string(*log_file_name),
              *max_file_size_mb * 1024 * 1024,
              *max_file_count
            )
          );
          break;
        case utils::to_underlying(Target::Stdout):
          sinks.push_back(make_shared<spdlog::sinks::stdout_color_sink_mt>());
          break;
        default: unreachable();
      }
      bitmask &= compl mask;
      mask <<= 1;
    }

    const auto l = make_shared<spdlog::logger>(logger_name.data(), begin(sinks), end(sinks));
    spdlog::set_default_logger(l);
    spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
    spdlog::set_pattern(string(log_pattern));

    spdlog::flush_on(spdlog::level::debug);
    spdlog::flush_every(1s);

    llog::debug("logger {} initialized", logger_name);
    llog::debug("logger {} level: {}", logger_name, string(magic_enum::enum_name(level)));
    llog::debug("logger {} target: {}", logger_name, string(magic_enum::enum_name(target)));
    if(log_file_name.has_value())
      llog::debug("logger {} log file: {}", logger_name, string(*log_file_name));
    if(max_file_size_mb.has_value())
      llog::debug("logger {} max file size: {}", logger_name, *max_file_size_mb);
    if(max_file_count.has_value())
      llog::debug("logger {} max file count: {}", logger_name, *max_file_count);

    l->flush();
    this->initialized = true;
  }
}