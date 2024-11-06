#include <rolly/directories.h>
#include <catch2/catch_all.hpp>
#include <rolly/global/platform_definitions.h>

using namespace rolly;

#if 0
TEST(Directories, ApplicationDirs)
{
  using dir = application_dirs::dir;

  auto dirs = application_dirs("com", "Foo Corp", "Bar App");
#  if defined(ROLLY_OS_WINDOWS)
#    define _CRT_SECURE_NO_WARNINGS
  auto username = std::string(std::getenv("USERNAME"));
  EXPECT_EQ(dirs[dir::cache].string(), R"(C:\Users\)" + username + R"(\AppData\Local\Foo Corp\Bar App\cache)");
  EXPECT_EQ(dirs[dir::config].string(), R"(C:\Users\)" + username + R"(\AppData\Roaming\Foo Corp\Bar App\config)");
  EXPECT_EQ(dirs[dir::data].string(), R"(C:\Users\)" + username + R"(\AppData\Roaming\Foo Corp\Bar App\data)");
  EXPECT_EQ(dirs[dir::data_local].string(), R"(C:\Users\)" + username + R"(\AppData\Local\Foo Corp\Bar App\data)");
  EXPECT_EQ(dirs[dir::config_local].string(), R"(C:\Users\)" + username + R"(\AppData\Local\Foo Corp\Bar App\config)");
  EXPECT_EQ(dirs[dir::preferences].string(), R"(C:\Users\)" + username + R"(\AppData\Roaming\Foo Corp\Bar App\config)");
  EXPECT_FALSE(dirs.runtime_dir().has_value());
  EXPECT_FALSE(dirs.state_dir().has_value());
#  else
  GTEST_SUCCEED();
#  endif
}
#endif
