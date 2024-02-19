#include <gtest/gtest.h>
#include <leaf/global.h>
#include <leaf/conversion.h>

TEST(Spdlog, Basic)
{
  llog::error("error message from spdlog");
}

TEST(Conversion, FixedSizeIntegers)
{
  using std::string;
  using leaf::conversion::into;
  using namespace leaf::types;

  const auto a = into<u8, string>(u8{0});
  const auto b = into<i8, string>(i8{-10});
  const auto c = into<u16, string>(u16{12312});
  const auto d = into<i16, string>(i16{-12413});
  const auto e = into<u32, string>(u32{123123});
  const auto f = into<i32, string>(i32{-123123});
  const auto g = into<u64, string>(u64{23234234234});
  const auto h = into<i64, string>(i64{-255345345});
  const auto i = into<u8, string>(u8{0});
  const auto j = into<u8, string>(u8{14});
  const auto k = into<u8, string>(u8{26});
  const auto l = into<u8, string>(u8{145});
  const auto m = into<u8, string>(u8{255});
  const auto n = into<u8, string>(u8{17});
  const auto o = into<u8, string>(u8{91});
  const auto p = into<u8, string>(u8{100});

  ASSERT_EQ(a, "0");
  ASSERT_EQ(b, "-10");
  ASSERT_EQ(c, "12312");
  ASSERT_EQ(d, "-12413");
  ASSERT_EQ(e, "123123");
  ASSERT_EQ(f, "-123123");
  ASSERT_EQ(g, "23234234234");
  ASSERT_EQ(h, "-255345345");
  ASSERT_EQ(i, "0");
  ASSERT_EQ(j, "14");
  ASSERT_EQ(k, "26");
  ASSERT_EQ(l, "145");
  ASSERT_EQ(m, "255");
  ASSERT_EQ(n, "17");
  ASSERT_EQ(o, "91");
  ASSERT_EQ(p, "100");
}

auto main(int argc, char** argv) -> int
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
