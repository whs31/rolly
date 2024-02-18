#include <gtest/gtest.h>
#include <leaf/global.h>

TEST(Spdlog, Basic)
{
  llog::error("error message from spdlog");
}

auto main(int argc, char** argv) -> int
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
