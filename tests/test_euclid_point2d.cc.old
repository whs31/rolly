#include <cstdint>
#include <vector>
#include <numeric>
#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/euclid.h>

using fl::math::size2d;
using fl::math::point2d;
using fl::math::vector2d;
using fl::math::default_unit;
using fl::math::scale;
using namespace fl::types;

struct Mm {};
struct Cm {};

TEST(EuclidPoint2D, Negation)
{
  EXPECT_EQ(-point2d(1.0, 2.0), point2d(-1.0, -2.0));
  EXPECT_EQ(-point2d(0.0, 0.0), point2d(-0.0, -0.0));
  EXPECT_EQ(-point2d(-1.0, -2.0), point2d(1.0, 2.0));
}

TEST(EuclidPoint2D, AddSize)
{
  auto const p1 = point2d(1.0, 2.0);
  auto const p2 = size2d(3.0, 4.0);

  EXPECT_EQ(p1 + p2, point2d(4.0, 6.0));
}

TEST(EuclidPoint2D, AddAssignSize)
{
  auto p1 = point2d(1.0, 2.0);
  auto const p2 = size2d(3.0, 4.0);

  EXPECT_EQ(p1 += p2, point2d(4.0, 6.0));
}

TEST(EuclidPoint2D, Subtract)
{
  auto const p1 = point2d(1.0, 2.0);
  auto const p2 = point2d(3.0, 4.0);

  EXPECT_EQ(p1 - p2, point2d(-2.0, -2.0));
}

TEST(EuclidPoint2D, SubtractAssign)
{
  auto p1 = point2d(1.0, 2.0);
  auto const p2 = point2d(3.0, 4.0);

  EXPECT_EQ(p1 -= p2, point2d(-2.0, -2.0));
}

TEST(EuclidPoint2D, SubtractSize)
{
  auto const p1 = point2d(1.0, 2.0);
  auto const p2 = size2d(3.0, 4.0);

  EXPECT_EQ(p1 - p2, point2d(-2.0, -2.0));
}

TEST(EuclidPoint2D, SubtractAssignSize)
{
  auto p1 = point2d(1.0, 2.0);
  auto const p2 = size2d(3.0, 4.0);

  EXPECT_EQ(p1 -= p2, point2d(-2.0, -2.0));
}

TEST(EuclidPoint2D, MulScalar)
{
  auto const p1 = point2d(3.0, 5.0);

  EXPECT_EQ(p1 * 2.0, point2d(6.0, 10.0));
  EXPECT_EQ(p1 * 5.0, point2d(15.0, 25.0));
  EXPECT_EQ(p1 * -1.0, point2d(-3.0, -5.0));
}

TEST(EuclidPoint2D, MulAssignScalar)
{
  auto p1 = point2d(3.0, 5.0);

  EXPECT_EQ(p1 *= 2.0, point2d(6.0, 10.0));
  EXPECT_EQ(p1 *= 2.0, point2d(12.0, 20.0));
  EXPECT_EQ(p1 *= -0.5, point2d(-6.0, -10.0));
}

TEST(EuclidPoint2D, MulScale)
{
  auto const p1 = point2d<Mm>(1.0, 2.0);
  auto const cm_per_mm = scale<Mm, Cm>(0.1);

  EXPECT_EQ(p1 * cm_per_mm, point2d<Cm>(0.1, 0.2));
}

TEST(EuclidPoint2D, DivScalar)
{
  auto const p1 = point2d(3.0, 5.0);

  EXPECT_EQ(p1 / 2.0, point2d(1.5, 2.5));
  EXPECT_EQ(p1 / 5.0, point2d(0.6, 1.0));
  EXPECT_EQ(p1 / -1.0, point2d(-3.0, -5.0));
}

TEST(EuclidPoint2D, DivAssignScalar)
{
  auto p1 = point2d(3.0, 5.0);

  EXPECT_EQ(p1 /= 2.0, point2d(1.5, 2.5));
  EXPECT_EQ(p1 /= 2.0, point2d(0.75, 1.25));
}

TEST(EuclidPoint2D, DivScale)
{
  auto const p1 = point2d<Cm>(0.1, 0.2);
  auto const cm_per_mm = scale<Mm, Cm>(0.1);

  EXPECT_EQ(p1 / cm_per_mm, point2d<Mm>(1.0, 2.0));
}

TEST(EuclidPoint2D, Format)
{
  auto const p1 = point2d(1.0, 2.0);
  auto const res = fmt::format("{}", p1);

  EXPECT_TRUE(res == "[1, 2]" or res == "[1.0, 2.0]");
}

TEST(EuclidPoint2D, Equality)
{
  EXPECT_EQ(point2d(1.0, 2.0), point2d(1.0, 2.0));
  EXPECT_NE(point2d(1.0, 2.0), point2d(1.0, 3.0));
  EXPECT_NE(point2d(1.0, 2.0), point2d(2.0, 2.0));
}

TEST(EuclidPoint2D, RemEuclid)
{
  auto const p = point2d(7.0, -7.0);
  auto const sp = point2d(4.0, -4.0);
  auto const s = size2d(4.0, -4.0);

  EXPECT_EQ(p.rem_euclid(sp), point2d(3.0, 1.0));
  EXPECT_EQ((-p).rem_euclid(sp), point2d(1.0, 3.0));
  EXPECT_EQ(p.rem_euclid(-sp), point2d(3.0, 1.0));

  EXPECT_EQ(p.rem_euclid(s), point2d(3.0, 1.0));
  EXPECT_EQ((-p).rem_euclid(s), point2d(1.0, 3.0));
  EXPECT_EQ(p.rem_euclid(-s), point2d(3.0, 1.0));
}

TEST(EuclidPoint2D, DivEuclid)
{
  auto const p = point2d(7.0, -7.0);
  auto const sp = point2d(4.0, -4.0);
  auto const s = sp.to_size2d();

  EXPECT_EQ(p.div_euclid(sp), point2d(1.0, 2.0));
  EXPECT_EQ((-p).div_euclid(sp), point2d(-2.0, -1.0));
  EXPECT_EQ(p.div_euclid(-sp), point2d(-1.0, -2.0));

  EXPECT_EQ(p.div_euclid(s), point2d(1.0, 2.0));
  EXPECT_EQ((-p).div_euclid(s), point2d(-2.0, -1.0));
  EXPECT_EQ(p.div_euclid(-s), point2d(-1.0, -2.0));
}

#if defined(FL_QT_CORE)
TEST(EuclidPoint2D, ToQPoint)
{
  auto const p = point2d(1.0, 2.0);

  EXPECT_EQ(p.to_qpoint(), QPoint(1, 2));
}

TEST(EuclidPoint2D, ToQPointF)
{
  auto const p = point2d(1.0, 2.0);

  EXPECT_EQ(p.to_qpointf(), QPointF(1.0, 2.0));
}

TEST(EuclidPoint2D, FromQPoint)
{
  auto const p = QPoint(1, 2);

  EXPECT_EQ(point2d(p), point2d(1.0F, 2.0F));
}
#endif // FL_QT_CORE

TEST(EuclidPoint2D, Map)
{
  auto const p = point2d(1.0, 2.0);
  auto const expected = point2d<default_unit, i32>(2, 4);
  auto fn = [](f64 x) -> i32 { return x * 2; };
  EXPECT_EQ(p.map(fn), expected);
}

TEST(EuclidPoint2D, Zip)
{
  auto const p = point2d(1.0, 2.0);
  auto const q = point2d(3.0, 4.0);
  auto const got = p.zip(q, [](f64 x, f64 y) -> f64 { return x + y; });
  auto const expected = point2d(4.0, 6.0);
  EXPECT_EQ(got.to_size2d(), expected.to_size2d());
}

TEST(EuclidPoint2D, AddVec)
{
  auto const p = point2d(1.0, 2.0);
  auto const v = vector2d(3.0, 4.0);
  auto const got = p + v;
  auto const expected = point2d(4.0, 6.0);
  EXPECT_EQ(got, expected);
}

TEST(EuclidPoint2D, SubVec)
{
  auto const p = point2d(1.0, 2.0);
  auto const v = vector2d(3.0, 4.0);
  auto const got = p - v;
  auto const expected = point2d(-2.0, -2.0);
  EXPECT_EQ(got, expected);
}
