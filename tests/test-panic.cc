#include <gtest/gtest.h>
#include <lf/leaf.h>

TEST(Panic, Basic)
{
  ASSERT_DEATH(lf::panic("test panic"), "terminate called without an active exception");
}