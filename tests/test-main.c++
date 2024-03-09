#include <gtest/gtest.h>
#include <leaf/global.h>
#include <leaf/conversion.h>
#include <leaf/logger.h>

TEST(Spdlog, Basic)
{
  llog::error("error message from spdlog");

  ASSERT_EQ(1, 1);
}

TEST(Logger, Basic)
{
  const auto logger = leaf::Logger(
    "test-logger",
    leaf::Logger::DefaultPatterns::SimpleWithThreadInfo,
    leaf::Logger::Level::Trace,
    leaf::Logger::Target::Stdout | leaf::Logger::Target::File,
    "test.log",
    10.0f,
    1
  );

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
