#include <cstdint>
#include <numeric>
#include <vector>
#include <gtest/gtest.h>
#include <floppy/floppy.h>
#include <floppy/euclid/angle.h>

using fl::math::angle;
using namespace fl::types;

TEST(EuclidAngle, WrapAngles)
{
  auto constexpr PI = std::numbers::pi_v<f32>;
  auto constexpr FRAC_PI_2 = PI / 2.0F;

  EXPECT_EQ(angle<f32>::from_radians(0.0).positive(), angle<f32>::zero());
  EXPECT_EQ(angle<f32>::from_radians(FRAC_PI_2).positive(), angle<f32>::half_pi());
  EXPECT_EQ(angle<f32>::from_radians(-FRAC_PI_2).positive(), angle<f32>::from_radians(3.0F * FRAC_PI_2));
  EXPECT_EQ(angle<f32>::from_radians(3.0F * FRAC_PI_2).positive(), angle<f32>::from_radians(3.0F * FRAC_PI_2));
  EXPECT_EQ(angle<f32>::from_radians(5.0F * FRAC_PI_2).positive(), angle<f32>::half_pi());
  EXPECT_EQ(angle<f32>::from_radians(2.0 * PI).positive(), angle<f32>::zero());
  EXPECT_EQ(angle<f32>::from_radians(-2.0 * PI).positive(), angle<f32>::zero());
  EXPECT_EQ(angle<f32>::from_radians(PI).positive(), angle<f32>::pi());
  EXPECT_EQ(angle<f32>::from_radians(-PI).positive(), angle<f32>::pi());
  EXPECT_EQ(angle<f32>::from_radians(FRAC_PI_2).signed_(), angle<f32>::half_pi());
  EXPECT_TRUE(std::abs(*angle<f32>::from_radians(5.0F * FRAC_PI_2).signed_() - *angle<f32>::half_pi()) < 0.0001F);
  EXPECT_TRUE(std::abs(*angle<f32>::from_radians(3.0F * FRAC_PI_2).signed_() + *angle<f32>::half_pi()) < 0.0001F);
  EXPECT_EQ(angle<f32>::from_radians(2.0 * PI).signed_(), angle<f32>::zero());
  EXPECT_EQ(angle<f32>::from_radians(-2.0 * PI).signed_(), angle<f32>::zero());
  EXPECT_EQ(angle<f32>::from_radians(PI).signed_(), angle<f32>::pi());
  EXPECT_EQ(angle<f32>::from_radians(-PI).signed_(), angle<f32>::pi());
}

TEST(EuclidAngle, Lerp)
{
  auto const a = angle<f32>::from_radians(1.0F);
  auto const b = angle<f32>::from_radians(2.0F);

  EXPECT_FLOAT_EQ(*a.lerp(b, 0.25F), 1.25F);
  EXPECT_FLOAT_EQ(*a.lerp(b, 0.5F), 1.5F);
  EXPECT_FLOAT_EQ(*a.lerp(b, 0.75F), 1.75F);
  EXPECT_FLOAT_EQ(*a.lerp(b + 2.0F * std::numbers::pi, 0.75F), 1.75F);
  EXPECT_FLOAT_EQ(*a.lerp(b - 2.0F * std::numbers::pi, 0.75F), 1.75F);
  EXPECT_FLOAT_EQ(*a.lerp(b + 10.0F * std::numbers::pi, 0.75F), 1.75F);
}

TEST(EuclidAngle, Sum)
{
  auto const angles = std::vector<angle<f32>>{ angle<f32>::from_radians(1.0F), angle<f32>::from_radians(2.0F), angle<f32>::from_radians(3.0F) };
  auto const sum = angle<f32>::from_radians(6.0F);

  EXPECT_EQ(angles[0] + angles[1] + angles[2], sum);
  EXPECT_EQ(std::accumulate(angles.cbegin(), angles.cend(), angle<f32>::zero()), sum);
}

TEST(EuclidAngle, Format)
{
  auto const a = angle<f32>::from_degrees(1.0F);
  std::stringstream ss;
  ss << a;
  EXPECT_EQ(ss.str(), "1.00°");
  EXPECT_EQ(fmt::format("{}", a), "1.00°");
  //EXPECT_EQ(fmt::format("{}", a.cast<i32>()), "1°"); todo 17.06.2024: this test fails to compile for some strange reason
}