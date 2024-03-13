#include <gtest/gtest.h>
#include <leaf/global.h>
#include <leaf/conversion.h>
#include <leaf/logger.h>
#include <leaf/pattern/iobservable.h>

TEST(Spdlog, Basic)
{
  llog::error("error message from spdlog");

  ASSERT_EQ(1, 1);
}

class TestObservable : public leaf::pattern::IObservable<>
{
  public:
    TestObservable() { this->notify(); }
};

auto test_empty_variadic_args = 0;
class TestObserver final : public leaf::pattern::IObserver<>
{
  public:
    TestObserver() = default;

    virtual auto update() -> void override
    {
      printf("update triggered!");
      test_empty_variadic_args = 1;
    }
};

TEST(Observable, EmptyVariadicArgs)
{
  auto observable = TestObservable();
  auto observer = TestObserver();
  observable += &observer;
  observable.notify();
  observable -= &observer;
  EXPECT_EQ(test_empty_variadic_args, 1);
}

TEST(Logger, Basic)
{
  const auto logger = leaf::LoggerBuilder()
    .with_name("test-log-builder-logger")
    .with_pattern(leaf::Logger::DefaultPatterns::SimpleWithThreadInfo)
    .with_level(leaf::Logger::Level::Trace)
    .with_target(leaf::Logger::Target::Stdout | leaf::Logger::Target::File)
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
