#include <cstdint>
#include <vector>
#include <numeric>
#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/euclid.h>

using fl::math::size2d;
using fl::math::point2d;
using fl::math::default_unit;
using fl::math::scale;
using namespace fl::types;

struct Cm {};
struct Mm {};

TEST(EuclidSize2D, Area)
{
  auto const p = size2d(1.5, 2.0);

  EXPECT_EQ(p.x(), 1.5);
  EXPECT_EQ(p.y(), 2.0);
  EXPECT_EQ(p.area(), 3.0);
}

TEST(EuclidSize2D, FromTuple)
{
  auto const p = size2d<default_unit, f64>::from_tuple(std::tuple<double, double>{1.5, 2.0});

  EXPECT_EQ(p.x(), 1.5);
  EXPECT_EQ(p.y(), 2.0);
  EXPECT_EQ(p.area(), 3.0);
}

TEST(EuclidSize2D, Negation)
{
  EXPECT_EQ(-size2d(1.0, 2.0), size2d(-1.0, -2.0));
  EXPECT_EQ(-size2d(0.0, 0.0), size2d(-0.0, -0.0));
  EXPECT_EQ(-size2d(-1.0, -2.0), size2d(1.0, 2.0));
}

TEST(EuclidSize2D, Add)
{
  EXPECT_EQ(size2d(1.0, 2.0) + size2d(3.0, 4.0), size2d(4.0, 6.0));
  EXPECT_EQ(size2d(1.0, 2.0) + size2d(0.0, 0.0), size2d(1.0, 2.0));
  EXPECT_EQ(size2d(1.0, 2.0) + size2d(-3.0, -4.0), size2d(-2.0, -2.0));
  EXPECT_EQ(size2d(0.0, 0.0) + size2d(0.0, 0.0), size2d(0.0, 0.0));
}

TEST(EuclidSize2D, AddAssign)
{
  EXPECT_EQ(size2d(1.0, 2.0) += size2d(3.0, 4.0), size2d(4.0, 6.0));
  EXPECT_EQ(size2d(1.0, 2.0) += size2d(0.0, 0.0), size2d(1.0, 2.0));
  EXPECT_EQ(size2d(1.0, 2.0) += size2d(-3.0, -4.0), size2d(-2.0, -2.0));
  EXPECT_EQ(size2d(0.0, 0.0) += size2d(0.0, 0.0), size2d(0.0, 0.0));
}

TEST(EuclidSize2D, Sum)
{
  auto vec = std::vector<size2d<default_unit, f32>>{size2d(1.0F, 2.0F), size2d(3.0F, 4.0F), size2d(5.0F, 6.0F)};
  auto acc = std::accumulate(vec.begin(), vec.end(), size2d());

  EXPECT_EQ(acc, size2d(9.0F, 12.0F));
}

TEST(EuclidSize2D, Subtract)
{
  EXPECT_EQ(size2d(1.0, 2.0) - size2d(3.0, 4.0), size2d(-2.0, -2.0));
  EXPECT_EQ(size2d(1.0, 2.0) - size2d(0.0, 0.0), size2d(1.0, 2.0));
  EXPECT_EQ(size2d(1.0, 2.0) - size2d(-3.0, -4.0), size2d(4.0, 6.0));
  EXPECT_EQ(size2d(0.0, 0.0) - size2d(0.0, 0.0), size2d(0.0, 0.0));
}

TEST(EuclidSize2D, SubtractAssign)
{
  EXPECT_EQ(size2d(1.0, 2.0) -= size2d(3.0, 4.0), size2d(-2.0, -2.0));
  EXPECT_EQ(size2d(1.0, 2.0) -= size2d(0.0, 0.0), size2d(1.0, 2.0));
  EXPECT_EQ(size2d(1.0, 2.0) -= size2d(-3.0, -4.0), size2d(4.0, 6.0));
  EXPECT_EQ(size2d(0.0, 0.0) -= size2d(0.0, 0.0), size2d(0.0, 0.0));
}

TEST(EuclidSize2D, MultiplyByScalar)
{
  EXPECT_EQ(size2d(1.0, 2.0) * 3.0, size2d(3.0, 6.0));
  EXPECT_EQ(size2d(1.0, 2.0) * 0.0, size2d(0.0, 0.0));
  EXPECT_EQ(size2d(1.0, 2.0) * -3.0, size2d(-3.0, -6.0));
  EXPECT_EQ(size2d(0.0, 0.0) * 0.0, size2d(0.0, 0.0));
}

TEST(EuclidSize2D, MultiplyAssignByScalar)
{
  EXPECT_EQ(size2d(1.0, 2.0) *= 3.0, size2d(3.0, 6.0));
  EXPECT_EQ(size2d(1.0, 2.0) *= 0.0, size2d(0.0, 0.0));
  EXPECT_EQ(size2d(1.0, 2.0) *= -3.0, size2d(-3.0, -6.0));
  EXPECT_EQ(size2d(0.0, 0.0) *= 0.0, size2d(0.0, 0.0));
}

TEST(EuclidSize2D, MultiplyByScale)
{
  auto const sz = size2d<Mm>(1.0, 2.0);
  auto const cm_per_mm = scale<Mm, Cm>(0.1);
  EXPECT_EQ(sz * cm_per_mm, size2d<Cm>(0.1, 0.2));
}

TEST(EuclidSize2D, DivideByScalar)
{
  EXPECT_EQ(size2d(1.0, 2.0) / 2.0, size2d(0.5, 1.0));
  EXPECT_EQ(size2d(1.0, 2.0) / -2.0, size2d(-0.5, -1.0));
}

TEST(EuclidSize2D, DivideAssignByScalar)
{
  EXPECT_EQ(size2d(1.0, 2.0) /= 2.0, size2d(0.5, 1.0));
  EXPECT_EQ(size2d(1.0, 2.0) /= -2.0, size2d(-0.5, -1.0));
}

TEST(EuclidSize2D, DivideByScale)
{
  auto const sz = size2d<Cm>(0.1, 0.2);
  auto const cm_per_mm = scale<Mm, Cm>(0.1);

  EXPECT_EQ(sz / cm_per_mm, size2d<Mm>(1.0, 2.0));
}

TEST(EuclidSize2D, NanEmpty)
{
  auto const a = size2d(std::numeric_limits<f32>::quiet_NaN(), 2.0F);
  auto const b = size2d(0.0F, std::numeric_limits<f32>::quiet_NaN());
  auto const c = size2d(std::numeric_limits<f32>::quiet_NaN(), -2.0F);
  EXPECT_FALSE(a);
  EXPECT_FALSE(b);
  EXPECT_FALSE(c);
}

#if defined(FL_QT_CORE)
TEST(EuclidSize2D, ToQSize)
{
  EXPECT_EQ(size2d(1.0, 2.0).to_qsize(), QSize(1, 2));
  EXPECT_EQ(size2d(0.0, 0.0).to_qsize(), QSize(0, 0));
  EXPECT_EQ(size2d(-1.0, -2.0).to_qsize(), QSize(-1, -2));
}

TEST(EuclidSize2D, ToQSizeF)
{
  EXPECT_EQ(size2d(1.0, 2.0).to_qsizef(), QSizeF(1.0, 2.0));
  EXPECT_EQ(size2d(0.0, 0.0).to_qsizef(), QSizeF(0.0, 0.0));
  EXPECT_EQ(size2d(-1.0, -2.0).to_qsizef(), QSizeF(-1.0, -2.0));
}
#endif // defined(FL_QT_CORE)

TEST(EuclidSize2D, ToPoint2D)
{
  EXPECT_EQ(size2d(1.0, 2.0).to_point2d(), point2d(1.0, 2.0));
  EXPECT_EQ(size2d(0.0, 0.0).to_point2d(), point2d(0.0, 0.0));
  EXPECT_EQ(size2d(-1.0, -2.0).to_point2d(), point2d(-1.0, -2.0));
}