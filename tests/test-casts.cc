#include <gtest/gtest.h>
#include <lf/leaf.h>

using namespace lf::types;
using namespace lf::literals;

TEST(CoreFunctions, Casts)
{
  auto a = 1_u8;
  auto const& b = a;

  EXPECT_EQ(lf::narrow_cast<u16>(a), 1);
  EXPECT_EQ(lf::narrow_cast<u64>(b), lf::narrow_cast<i64>(a));
}