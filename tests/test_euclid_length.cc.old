#include <cstdint>
#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/euclid/length.h>

using fl::math::length;
using fl::math::scale;
using namespace fl::types;

struct Inch {};
struct Mm {};
struct Cm {};
struct Second {};

TEST(EuclidLength, Clone)
{
  auto mut_len = length<Inch>(12.0F);
  auto const len = mut_len;
  mut_len = 24.0F;

  EXPECT_FLOAT_EQ(*len, 12.0F);
  EXPECT_FLOAT_EQ(*mut_len, 24.0F);
}

TEST(EuclidLength, Add)
{
  auto const a = length<Mm, u8>(250);
  auto const b = length<Mm, u8>(5);

  EXPECT_EQ(*(a + b), 255);
  EXPECT_EQ(*(a + 5), 255);
}

TEST(EuclidLength, AddAssign)
{
  auto const one_cm = length<Mm>(10.0F);
  auto measurement = length<Mm>(5.0F);
  measurement += one_cm;

  EXPECT_FLOAT_EQ(*measurement, 15.0F);
}

TEST(EuclidLength, Sub)
{
  auto const a = length<Mm, u8>(250);
  auto const b = length<Mm, u8>(5);

  EXPECT_EQ(*(a - b), 245);
}

TEST(EuclidLength, SubAssign)
{
  auto const one_cm = length<Mm>(10.0F);
  auto measurement = length<Mm>(5.0F);
  measurement -= one_cm;

  EXPECT_FLOAT_EQ(*measurement, -5.0F);
}

TEST(EuclidLength, DivisionByLength)
{
  auto const l = length<Cm>(5.0F);
  auto const d = length<Second>(10.0F);
  auto const result = l / d;
  auto const expected = scale<Second, Cm>(0.5F);
  EXPECT_EQ(result, expected);
}

TEST(EuclidLength, Multiplication)
{
  auto const l_mm = length<Mm>(10.0F);
  auto const cm_per_mm = scale<Mm, Cm>(0.1F);
  auto const result = l_mm * cm_per_mm;
  auto const expected = length<Cm>(1.0F);
  EXPECT_EQ(result, expected);
}

TEST(EuclidLength, MultiplicationWithScalar)
{
  auto const l_mm = length<Mm>(10.0F);
  auto const result = l_mm * 2.0F;
  auto const expected = length<Mm>(20.0F);
  EXPECT_EQ(result, expected);
}

TEST(EuclidLength, MultiplicationAssignment)
{
  auto mut_len = length<Mm>(10.0F);
  mut_len *= 2.0F;
  auto const expected = length<Mm>(20.0F);
  EXPECT_EQ(mut_len, expected);
}

TEST(EuclidLength, DivisionByScaleFactor)
{
  auto const l = length<Cm>(5.0F);
  auto const s = scale<Second, Cm>(10.0F);
  auto const result = l / s;
  auto const expected = length<Second>(0.5F);
  EXPECT_EQ(result, expected);
}

TEST(EuclidLength, DivisionByScalar)
{
  auto const l = length<Cm>(5.0F);
  auto const result = l / 2.0F;
  auto const expected = length<Cm>(2.5F);
  EXPECT_EQ(result, expected);
}

TEST(EuclidLength, DivisionAssignment)
{
  auto mut_len = length<Mm>(10.0F);
  mut_len /= 2.0F;
  auto const expected = length<Mm>(5.0F);
  EXPECT_EQ(mut_len, expected);
}

TEST(EuclidLength, Format)
{
  auto const l = length<Cm>(5.0F);
  EXPECT_EQ(fmt::format("{}", l), "5.000");
}

TEST(EuclidLength, Negation)
{
  auto const l = length<Cm>(5.0F);
  auto const result = -l;
  auto const expected = length<Cm>(-5.0F);
  EXPECT_EQ(result, expected);
}

TEST(EuclidLength, Cast)
{
  auto const l = length<Cm, i32>(5);
  auto const result = l.cast<f32>();
  auto const expected = length<Cm>(5.0F);
  EXPECT_EQ(result, expected);
}

TEST(EuclidLength, Equality)
{
  auto const l = length<Cm>(5.0F);
  auto const l2 = length<Cm>(5.1F);
  auto const l3 = length<Cm>(0.1F);

  EXPECT_TRUE(l == l2 - l3);
  EXPECT_TRUE(l != l2);
}

TEST(EuclidLength, Ordering)
{
  auto const l = length<Cm>(5.0F);
  auto const l2 = length<Cm>(5.1F);
  auto const l3 = length<Cm>(0.1F);

  EXPECT_TRUE(l < l2);
  EXPECT_TRUE(l <= l2);
  EXPECT_TRUE(l <= l2 - l3);
  EXPECT_TRUE(l2 > l);
  EXPECT_TRUE(l2 >= l);
  EXPECT_TRUE(l2 >= l - l3);
}


TEST(EuclidLength, ZeroAdd)
{
  auto const l = length<Cm>(5.0F);
  auto const l_zero = length<Cm>(0.0F);
  auto const result = l + l_zero;
  auto const expected = l - 0.0F;
  EXPECT_EQ(result, expected);
}

TEST(EuclidLength, DivisionByZero)
{
  auto const l = length<Cm>(5.0F);
  auto const l_zero = length<Cm>(0.0F);
  auto const result = l / l_zero;
  auto const expected = scale<Cm, Cm>(INFINITY);
  EXPECT_EQ(result, expected);
}