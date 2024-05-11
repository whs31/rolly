#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <magic_enum/magic_enum.hpp>
#include <leaf/logger.h>
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

using std::make_shared;

namespace leaf {
  Logger::Logger(
    bool const default_,
    string_view const logger_name,
    string_view const log_pattern,
    Level const level,
    Target const target,
    Option<string_view> const log_file_name,
    Option<f32> const max_file_size_mb,
    Option<usize> const max_file_count
  )
    : initialized(false)
    , m_logger_name_mt(logger_name)
  {
    if (log_pattern.empty()) {
      llog::warn("no log pattern provided to logger {}", logger_name);
      return;
    }

    vector<spdlog::sink_ptr> sinks;
    auto bitmask = utils::to_underlying(target);
    auto mask = 1;
    while (bitmask) {
      switch (bitmask bitand mask) {
        case utils::to_underlying(Target::File):
          if (not log_file_name.has_value()) {
            llog::error("no log file name provided to logger {}", logger_name);
            return;
          }
          if (not max_file_size_mb.has_value()) {
            llog::error("no max file size provided to logger {}", logger_name);
            return;
          }
          if (not max_file_count.has_value()) {
            llog::error("no max file count provided to logger {}", logger_name);
            return;
          }
          sinks.push_back(make_shared<spdlog::sinks::rotating_file_sink_mt>(
            string(*log_file_name), *max_file_size_mb * 1024 * 1024, *max_file_count));
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
    if(default_)
      spdlog::set_default_logger(l);
    l->set_level(static_cast<spdlog::level::level_enum>(level));
    l->set_pattern(string(log_pattern));

    l->flush_on(spdlog::level::debug);
    spdlog::flush_every(1s);

    llog::debug("logger {} initialized", logger_name);
    llog::debug("logger {} level: {}", logger_name, string(magic_enum::enum_name(level)));
    llog::debug("logger {} target: {}", logger_name, string(magic_enum::enum_name(target)));
    if (log_file_name.has_value())
      llog::debug("logger {} log file: {}", logger_name, string(*log_file_name));
    if (max_file_size_mb.has_value())
      llog::debug("logger {} max file size: {}", logger_name, *max_file_size_mb);
    if (max_file_count.has_value())
      llog::debug("logger {} max file count: {}", logger_name, *max_file_count);

    l->flush();
    this->initialized = true;
  }

  Logger::~Logger()
  {
    llog::trace("logger {}: closing", this->m_logger_name_mt);
    spdlog::shutdown();
  }

  auto Logger::make_default() const -> void { spdlog::set_default_logger(this->logger()); }

  LoggerBuilder::LoggerBuilder()
    : level(Logger::Level::Info)
    , target(Logger::Target::Stdout)
    , default_(false)
  {
  }

  auto LoggerBuilder::with_name(string_view const n) -> LoggerBuilder&
  {
    this->name = n;
    return *this;
  }

  auto LoggerBuilder::with_pattern(const string_view p) -> LoggerBuilder&
  {
    this->pattern = p;
    return *this;
  }

  auto LoggerBuilder::with_level(const Logger::Level l) -> LoggerBuilder&
  {
    this->level = l;
    return *this;
  }

  auto LoggerBuilder::with_target(const Logger::Target t) -> LoggerBuilder&
  {
    this->target = t;
    return *this;
  }

  auto LoggerBuilder::with_log_file_name(string_view l) -> LoggerBuilder&
  {
    this->log_file_name = l;
    return *this;
  }

  auto LoggerBuilder::with_max_file_size_mb(types::f32 m) -> LoggerBuilder&
  {
    this->max_file_size_mb = m;
    return *this;
  }

  auto LoggerBuilder::with_max_file_count(types::usize m) -> LoggerBuilder&
  {
    this->max_file_count = m;
    return *this;
  }

  auto LoggerBuilder::with_default(bool const d) -> LoggerBuilder& {
    this->default_ = d;
    return *this;
  }

  auto LoggerBuilder::build() const -> Result<shared_ptr<Logger>>
  {
    if(this->name.empty())
      return Error("no logger name provided");
    if(this->pattern.empty())
      return Error("no log pattern provided");
    if((this->target bitand Logger::Target::File) == Logger::Target::File) {
      if(not this->log_file_name.has_value())
        return Error("no log file name provided, but target is set to log to file");
      if(not this->max_file_size_mb.has_value())
        return Error("no max file size provided, but target is set to log to file");
      if(not this->max_file_count.has_value())
        return Error("no max file count provided, but target is set to log to file");
    }
    return Ok(make_shared<Logger>(
      this->default_,
      this->name,
      this->pattern,
      this->level,
      this->target,
      this->log_file_name,
      this->max_file_size_mb,
      this->max_file_count
    ));
  }
} // namespace leaf
