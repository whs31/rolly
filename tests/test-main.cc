#include <gtest/gtest.h>
#include <lf/logger.h>

TEST(Logger, Basic)
{
  const auto logger = lf::LoggerBuilder()
    .with_default(true)
    .with_name("test-log-builder-logger")
    .with_pattern(lf::Logger::DefaultPatterns::SimpleWithThreadInfo)
    .with_level(lf::Logger::Level::Trace)
    .with_target(lf::Logger::Target::Stdout | lf::Logger::Target::File)
    .with_log_file_name("test.log")
    .with_max_file_count(1)
    .with_max_file_size_mb(10.0f)
    .build()
    .map_error([](auto error) {
      llog::error(error);
      return 1;
    }).value();

  llog::trace("trace message");
  llog::debug("debug message");
  llog::info("info message");
  llog::warn("warn message");
  llog::error("error message");
  llog::critical("fatal message");

  ASSERT_EQ(1, 1);
}

auto main(int argc, char** argv) -> int
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
