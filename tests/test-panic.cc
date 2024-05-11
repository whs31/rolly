#include <gtest/gtest.h>
#include <leaf/leaf.h>

TEST(Panic, Basic)
{
  ASSERT_DEATH(leaf::panic("test panic"), "terminate called without an active exception");
}