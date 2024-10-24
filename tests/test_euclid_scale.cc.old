#include <cstdint>
#include <gtest/gtest.h>
#include <floppy/euclid.h>

using fl::math::scale;
using namespace fl::types;

struct Inch {};
struct Cm {};
struct Mm {};

TEST(EuclidScale, Operators)
{
  auto const mm_per_inch = scale<Inch, Mm>(25.4F);
  auto const cm_per_mm = scale<Mm, Cm>(0.1F);
  auto const mm_per_cm = cm_per_mm.inverse();
  auto const one = cm_per_mm * mm_per_cm;

  EXPECT_FLOAT_EQ(*mm_per_cm, 10.0F);
  EXPECT_FLOAT_EQ(*one, 1.0F);
  EXPECT_FLOAT_EQ(*(mm_per_cm * cm_per_mm), 1.0F);
  //  mm     cm     cm
  // ---- x ---- = ----
  // inch    mm    inch
  EXPECT_FLOAT_EQ(*(mm_per_inch * cm_per_mm), 2.54F);
}

TEST(EuclidScale, Integral)
{
  auto const a = scale<Inch, Inch, isize>(2);
  auto const b = scale<Inch, Inch, isize>(3);
  auto const c = scale<Inch, Inch, isize>(5);
  auto const d = scale<Inch, Inch, isize>(-1);
  EXPECT_NE(a, b);
  EXPECT_EQ(a + b, c);
  EXPECT_EQ(a - b, d);
}

TEST(EuclidScale, Format)
{
  auto const cm_per_mm = scale<Mm, Cm>(0.1F);
  EXPECT_EQ(fmt::format("{}", cm_per_mm), "1/10");
}