#include <cstdint>
#include <vector>
#include <numeric>
#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/euclid.h>

using fl::math::size2d;
using fl::math::vector2d;
using fl::math::point2d;
using fl::math::default_unit;
using fl::math::scale;
using fl::math::angle;
using namespace fl::types;

struct Mm {};
struct Cm {};

TEST(EuclidVector2D, ScalarMul)
{
  auto const p1 = vector2d(3.0, 5.0);
  auto const result = p1 * 5.0;

  EXPECT_FLOAT_EQ(result.x(), 15.0);
  EXPECT_FLOAT_EQ(result.y(), 25.0);
  EXPECT_EQ(result, vector2d(15.0, 25.0));
}

TEST(EuclidVector2D, Dot)
{
  auto const p1 = vector2d(2.0, 7.0);
  auto const p2 = vector2d(13.0, 11.0);
  EXPECT_FLOAT_EQ(p1.dot(p2), 103.0);
}

TEST(EuclidVector2D, Cross)
{
  auto const p1 = vector2d(4.0, 7.0);
  auto const p2 = vector2d(13.0, 8.0);
  EXPECT_FLOAT_EQ(p1.cross(p2), -59.0);
}

TEST(EuclidVector2D, Normalize)
{
  auto const p1 = vector2d(4.0, 0.0);
  auto const p2 = vector2d(3.0, -4.0);
  EXPECT_FLOAT_EQ(p1.normalized().x(), 1.0);
  EXPECT_FLOAT_EQ(p1.normalized().y(), 0.0);
  EXPECT_FLOAT_EQ(p2.normalized().x(), 0.6);
  EXPECT_FLOAT_EQ(p2.normalized().y(), -0.8);
}

TEST(EuclidVector2D, Length)
{
  auto const p1 = vector2d(3.0, 4.0);

  EXPECT_FLOAT_EQ(p1.length_scalar(), 5.0);
}

TEST(EuclidVector2D, LengthSquared)
{
  auto const p1 = vector2d(3.0, 4.0);

  EXPECT_FLOAT_EQ(p1.length_squared(), 25.0);
}

TEST(EuclidVector2D, Min)
{
  auto const p1 = vector2d(1.0, 3.0);
  auto const p2 = vector2d(2.0, 2.0);

  EXPECT_EQ(p1.min(p2), vector2d(1.0, 2.0));
}

TEST(EuclidVector2D, Max)
{
  auto const p1 = vector2d(1.0, 3.0);
  auto const p2 = vector2d(2.0, 2.0);

  EXPECT_EQ(p1.max(p2), vector2d(2.0, 3.0));
}

TEST(EuclidVector2D, AngleFromXAxis)
{
  auto const right = vector2d(10.0, 0.0);
  auto const down = vector2d(0.0, 4.0);
  auto const up = vector2d(0.0, -1.0);

  EXPECT_EQ(right.angle_to_x_axis(), angle<f64>::zero());
  EXPECT_EQ(down.angle_to_x_axis(), angle<f64>::half_pi());
  EXPECT_EQ(up.angle_to_x_axis(), -angle<f64>::half_pi());
}

TEST(EuclidVector2D, AngleTo)
{
  auto const right = vector2d(10.0, 0.0);
  auto const right2 = vector2d(1.0, 0.0);
  auto const up = vector2d(0.0, -1.0);
  auto const up_left = vector2d(-1.0, -1.0);

  EXPECT_EQ(right.angle_to(right2), angle<f64>::zero());
  EXPECT_EQ(right.angle_to(up), -angle<f64>::half_pi());
  EXPECT_EQ(up.angle_to(right), angle<f64>::half_pi());
  EXPECT_EQ(up_left.angle_to(up), angle<f64>::quarter_pi());
}

TEST(EuclidVector2D, ProjectOnto)
{
  auto const v1 = vector2d(1.0, 2.0);
  auto const x = vector2d(1.0, 0.0);
  auto const y = vector2d(0.0, 1.0);

  EXPECT_EQ(v1.project(x), vector2d(1.0, 0.0));
  EXPECT_EQ(v1.project(y), vector2d(0.0, 2.0));
  EXPECT_EQ(v1.project(-x), vector2d(1.0, 0.0));
  EXPECT_EQ(v1.project(x * 10.0), vector2d(1.0, 0.0));
  EXPECT_EQ(v1.project(v1 * 2.0), v1);
  EXPECT_EQ(v1.project(-v1), v1);
}

TEST(EuclidVector2D, Add)
{
  auto const p1 = vector2d(1.0, 2.0);
  auto const p2 = vector2d(3.0, 4.0);

  EXPECT_EQ(p1 + p2, vector2d(4.0, 6.0));
  EXPECT_EQ(p1 + p2, vector2d(4.0, 6.0));
}

TEST(EuclidVector2D, Sum)
{
  auto vec = std::vector<vector2d<default_unit, f32>>{vector2d(1.0F, 2.0F), vector2d(3.0F, 4.0F), vector2d(5.0F, 6.0F)};
  auto acc = std::accumulate(vec.begin(), vec.end(), vector2d());

  EXPECT_EQ(acc, vector2d(9.0F, 12.0F));
}

TEST(EuclidVector2D, Sub)
{
  auto const p1 = vector2d(1.0, 2.0);
  auto const p2 = vector2d(3.0, 4.0);

  EXPECT_EQ(p1 - p2, vector2d(-2.0, -2.0));
}

TEST(EuclidVector2D, SubAssign)
{
  auto p1 = vector2d(1.0, 2.0);
  p1 -= vector2d(3.0, 4.0);

  EXPECT_EQ(p1, vector2d(-2.0, -2.0));
}

TEST(EuclidVector2D, MulAssign)
{
  auto p1 = vector2d(1.0, 2.0);
  p1 *= 3.0;

  EXPECT_EQ(p1, vector2d(3.0, 6.0));
}

TEST(EuclidVector2D, DivAssign)
{
  auto p1 = vector2d(3.0, 6.0);
  p1 /= 3.0;

  EXPECT_EQ(p1, vector2d(1.0, 2.0));
}

TEST(EuclidVector2D, Neg)
{
  auto p1 = vector2d(1.0, 2.0);
  auto p2 = -p1;

  EXPECT_EQ(p2, vector2d(-1.0, -2.0));
}

TEST(EuclidVector2D, Swizzle)
{
  auto p1 = vector2d(1.0, 2.0);

  EXPECT_EQ(p1.yx(), vector2d(2.0, 1.0));
}

TEST(EuclidVector2D, Reflect)
{
  auto const a = vector2d(1.0, 3.0);
  auto const n1 = vector2d(0.0, -1.0);
  auto const n2 = vector2d(1.0, -1.0).normalized();

  EXPECT_EQ(a.reflected(n1), vector2d(1.0, -3.0));
  EXPECT_EQ(a.reflected(n2), vector2d(3.0, 1.0));
}

TEST(EuclidVector2D, Map)
{
  auto const p = vector2d(1.0, 2.0);
  auto const expected = vector2d<default_unit, i32>(2, 4);
  auto fn = [](f64 x) -> i32 { return x * 2; };
  EXPECT_EQ(p.map(fn), expected);
}

TEST(EuclidVector2D, Zip)
{
  auto const p = vector2d(1.0, 2.0);
  auto const q = vector2d(3.0, 4.0);
  auto const got = p.zip(q, [](f64 x, f64 y) -> f64 { return x + y; });
  auto const expected = vector2d(4.0, 6.0);
  EXPECT_EQ(got.to_point2d(), expected.to_point2d());
}