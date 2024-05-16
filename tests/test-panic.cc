#include <gtest/gtest.h>
#include <lf/leaf.h>

TEST(CoreFunctions, Basic)
{
  ASSERT_DEATH(lf::panic("test panic"), "");
}