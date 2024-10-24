#include <rolly/bit.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace rolly;

TEST(Bit, IsPowerOfTwo)
{
  EXPECT_FALSE(is_pow2(0));
  EXPECT_TRUE(is_pow2(1));
  EXPECT_TRUE(is_pow2(2));
  EXPECT_TRUE(is_pow2(4));
  EXPECT_FALSE(is_pow2(5));
  EXPECT_TRUE(is_pow2(8));
  EXPECT_FALSE(is_pow2(9));
  EXPECT_TRUE(is_pow2(16));
  EXPECT_FALSE(is_pow2(17));
  EXPECT_TRUE(is_pow2(32));
  EXPECT_FALSE(is_pow2(33));
  EXPECT_TRUE(is_pow2(64));
  EXPECT_FALSE(is_pow2(65));
  EXPECT_TRUE(is_pow2(128));
  EXPECT_FALSE(is_pow2(129));
  EXPECT_TRUE(is_pow2(256));
  EXPECT_FALSE(is_pow2(257));
  EXPECT_TRUE(is_pow2(512));
  EXPECT_FALSE(is_pow2(513));
  EXPECT_TRUE(is_pow2(1024));
}