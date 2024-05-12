#pragma once

#include <lf/leaf.h>

namespace lf
{
  /**
   * \brief Класс-конфигуратор логгера.
   * \details Создает новый логгер с указанными параметрами и задает его как логгер по умолчанию.
   * \param default_ Является ли логгер логгером по умолчанию
   * \param logger_name Имя логгера
   * \param log_pattern Паттерн логгирования. Можно задать как кастомную строку, так и паттерн по умолчанию - см. Logger::DefaultPatterns
   * \param level Уровень логгирования. См. Logger::Level
   * \param target Цель логгирования. Можно выбрать несколько целей - см. Logger::Target
   * \param log_file_name - имя лог-файла (необходимо задавать только если одна из целей логгирования - файл).
   * \param max_file_size_mb - максимальный размер лог-файла в мегабайтах (необходимо задавать только если одна из целей логгирования - файл)
   * \param max_file_count - максимальное количество лог-файлов (необходимо задавать только если одна из целей логгирования - файл)
   */
  class LF_EXPORT Logger final
  {
   public:
    struct DefaultPatterns
    {
      constexpr static auto SimpleWithThreadInfo = "[%=8!l] [thread %=5!t]:%^\033[1m %v %$\033[m";
    };

    enum class Level
    {
      Trace,
      Debug,
      Info,
      Warn,
      Error,
      Fatal
    };

    enum class Target
    {
      Stdout = 1 << 0,
      File   = 1 << 1,
      All    = Stdout | File
    };

    explicit Logger(
      bool default_,
      std::string_view logger_name,
      std::string_view log_pattern,
      Level level,
      Target target,
      Option<std::string_view> log_file_name,
      Option<f32> max_file_size_mb,
      Option<usize> max_file_count
    );

    ~Logger();

    bool initialized;

    [[nodiscard]] inline auto logger() const -> std::shared_ptr<spdlog::logger> { return this->m_logger; }
    auto make_default() const -> void;

   private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::string m_logger_name_mt;
  };

  inline auto operator|(const Logger::Target& lhs, const Logger::Target& rhs) -> Logger::Target {
    return static_cast<Logger::Target>(static_cast<int>(lhs) | static_cast<int>(rhs));
  }

  inline auto operator&(const Logger::Target& lhs, const Logger::Target& rhs) -> Logger::Target {
    return static_cast<Logger::Target>(static_cast<int>(lhs) & static_cast<int>(rhs));
  }

  class LF_EXPORT LoggerBuilder final
  {
   public:
    LoggerBuilder();

    auto with_default(bool default_) -> LoggerBuilder&;
    auto with_name(std::string_view name) -> LoggerBuilder&;
    auto with_pattern(std::string_view pattern) -> LoggerBuilder&;
    auto with_level(Logger::Level level) -> LoggerBuilder&;
    auto with_target(Logger::Target target) -> LoggerBuilder&;
    auto with_log_file_name(std::string_view log_file_name) -> LoggerBuilder&;
    auto with_max_file_size_mb(types::f32 max_file_size_mb) -> LoggerBuilder&;
    auto with_max_file_count(types::usize max_file_count) -> LoggerBuilder&;

    [[nodiscard]] auto build() const -> Result<std::shared_ptr<Logger>>;

   private:
    bool default_;
    std::string name;
    std::string pattern;
    Logger::Level level;
    Logger::Target target;
    Option<std::string> log_file_name;
    Option<f32> max_file_size_mb;
    Option<usize> max_file_count;
  };
}