#include <rolly/types/guid.h>
#include <gtest/gtest.h>

#include <sstream>

using namespace rolly;

TEST(GUID, Random)
{
  auto const r1 = guid::random();
  auto const r2 = guid::random();
  auto const r3 = guid::random();

  EXPECT_NE(r1, r2);
  EXPECT_NE(r1, r3);
  EXPECT_NE(r2, r3);
}

TEST(GUID, Compare)
{
  auto const s1 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
  auto const s2 = guid("16d1bd03-09a5-47d3-944b-5e326fd52d27");
  auto const s3 = guid("fdaba646-e07e-49de-9529-4499a5580c75");
  auto const s4 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
  auto const s5 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3b31");

  EXPECT_EQ(s1, s4);
  EXPECT_NE(s1, s2);
  EXPECT_NE(s1, s3);
  EXPECT_NE(s1, s5);
}

TEST(GUID, Ostream)
{
  auto const s1 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
  auto sstream = std::stringstream();
  sstream << s1;

  EXPECT_EQ(sstream.str(), "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
}

TEST(GUID, ToString)
{
  auto const s1 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");

  EXPECT_EQ(s1.to_string(), "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
}

TEST(GUID, Format)
{
  auto const s1 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");

  EXPECT_EQ(fmt::format("{}", s1), "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
}

TEST(GUID, FailParse)
{
  auto const s1 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3baskdfmsadf3e");

  EXPECT_FALSE(s1.valid());
  EXPECT_FALSE(s1);
}

TEST(GUID, Literal)
{
  auto const s1 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
  EXPECT_EQ(s1, "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e"_guid);
}
