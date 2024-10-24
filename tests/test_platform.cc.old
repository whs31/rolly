#include <cstdint>
#include <gtest/gtest.h>
#include <floppy/floppy.h>

TEST(Platform, TargetTripleString)
{
  auto const platform_info = fl::platform(
    fl::platform::operating_system::windows,
    fl::platform::arch::x86_64,
    fl::platform::compiler::msvc,
    fl::platform::endianness::little,
    20,
    '\\',
    ';'
  );
  EXPECT_EQ(platform_info.as_target_triple<char>(), "x86_64-pc-windows-msvc");
  EXPECT_EQ(fmt::format("{}", platform_info), "x86_64-pc-windows-msvc");
}