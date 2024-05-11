#include <gtest/gtest.h>
#include <leaf/leaf.h>

using namespace leaf::types;

auto fn_void(int x) -> Result<>
{
  if(x > 0)
    return Error("x is positive, should be negative ({})", x);
  return Ok();
}

auto fn(int x) -> Result<int>
{
  Result<int> i;
  Result<> r;
  if(x > 0)
    return Error("x is positive, should be negative ({})", x);
  return Ok(-x);
}

auto fn2(int x) -> Option<int>
{
  if(x > 0)
    return Some(-x);
  return None;
}

TEST(Expected, Basic)
{
  auto r = fn(1);
  ASSERT_TRUE(not r.has_value());
  ASSERT_EQ(r.error(), "x is positive, should be negative (1)");

  auto o = fn2(1);
  ASSERT_TRUE(o.has_value());
  ASSERT_EQ(*o, -1);

  o = fn2(-1);
  ASSERT_TRUE(not o.has_value());
  ASSERT_EQ(o, None);

  auto v = fn_void(-1);
  ASSERT_TRUE(v.has_value());
  ASSERT_EQ(v, Ok());
  v = fn_void(1);
  ASSERT_TRUE(not v.has_value());
  ASSERT_EQ(v.error(), "x is positive, should be negative (1)");
}