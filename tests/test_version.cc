#include <rolly/global/version.h>

#include <array>
#include <gtest/gtest.h>

using namespace rolly;

TEST(Version, ConstructorDefault)
{
  auto constexpr v = version();

  ASSERT_EQ(v.major, 0);
  ASSERT_EQ(v.minor, 1);
  ASSERT_EQ(v.patch, 0);
  ASSERT_EQ(v.prerelease_type, prerelease::none);
  ASSERT_FALSE(v.prerelease_number.has_value());
}

TEST(Version, Constructor)
{
  {
    auto constexpr v = version(1, 2, 3, prerelease::rc);

    ASSERT_EQ(v.major, 1);
    ASSERT_EQ(v.minor, 2);
    ASSERT_EQ(v.patch, 3);
    ASSERT_EQ(v.prerelease_type, prerelease::rc);
    ASSERT_FALSE(v.prerelease_number.has_value());
  }
  {
    auto constexpr v = version(1, 2, 3, prerelease::rc, 4);

    ASSERT_EQ(v.major, 1);
    ASSERT_EQ(v.minor, 2);
    ASSERT_EQ(v.patch, 3);
    ASSERT_EQ(v.prerelease_type, prerelease::rc);
    ASSERT_TRUE(v.prerelease_number.has_value());
    ASSERT_EQ(*v.prerelease_number, 4);
  }
  {
    auto constexpr v = version(1, 2, 3, prerelease::none);

    ASSERT_EQ(v.major, 1);
    ASSERT_EQ(v.minor, 2);
    ASSERT_EQ(v.patch, 3);
    ASSERT_EQ(v.prerelease_type, prerelease::none);
    ASSERT_FALSE(v.prerelease_number.has_value());
  }
  {
    auto constexpr v = version(1, 2, 3, prerelease::none, 0);

    ASSERT_EQ(v.major, 1);
    ASSERT_EQ(v.minor, 2);
    ASSERT_EQ(v.patch, 3);
    ASSERT_EQ(v.prerelease_type, prerelease::none);
    ASSERT_FALSE(v.prerelease_number.has_value());
  }
  {
    auto constexpr v = version(1, 2, 3, prerelease::none, 4);

    ASSERT_EQ(v.major, 1);
    ASSERT_EQ(v.minor, 2);
    ASSERT_EQ(v.patch, 3);
    ASSERT_EQ(v.prerelease_type, prerelease::none);
    ASSERT_FALSE(v.prerelease_number.has_value());
  }
}

TEST(Version, Parse)
{
  auto constexpr v = version("1.2.3-alpha.4");

  ASSERT_EQ(v.major, 1);
  ASSERT_EQ(v.minor, 2);
  ASSERT_EQ(v.patch, 3);
  ASSERT_EQ(v.prerelease_type, prerelease::alpha);
  ASSERT_TRUE(v.prerelease_number.has_value());
  ASSERT_EQ(*v.prerelease_number, 4);
  ASSERT_EQ(v.to_string(), "1.2.3-alpha.4");

  auto constexpr v2 = version("1.2.3-alpha.0");
  ASSERT_EQ(v2.major, 1);
  ASSERT_EQ(v2.minor, 2);
  ASSERT_EQ(v2.patch, 3);
  ASSERT_EQ(v2.prerelease_type, prerelease::alpha);
  ASSERT_TRUE(v2.prerelease_number.has_value());
  ASSERT_EQ(*v2.prerelease_number, 0);
  ASSERT_EQ(v2.to_string(), "1.2.3-alpha.0");
}

TEST(Version, ParseFail)
{
  ASSERT_ANY_THROW(auto v = version("1.2.3-asd"));
}

TEST(VersionRange, Satisfies)
{
  using namespace std::string_view_literals;

  constexpr auto r1 = ">1.2.3-alpha.3"sv;
  constexpr auto r2 = ">=1.2.3 < 2.0.0"sv;
  constexpr auto r3 = ">=1.2.3-alpha.7 <2.0.0"sv;
  constexpr auto r4 = ">1.2.3 <2.0.0-alpha.10"sv;
  constexpr auto r5 = ">1.2.3 <2.0.0-alpha.1 || <=2.0.0-alpha.5"sv;
  constexpr auto r6 = "<=2.0.0-alpha.4"sv;
  constexpr auto r7 = ">=2.0.0-alpha"sv;
  constexpr auto r8 = "<2.0.0-alpha"sv;

  constexpr version v1{"1.2.3-alpha.7"};
  constexpr version v2{"3.4.5-alpha.9"};
  constexpr version v3{"3.4.5"};
  constexpr version v4{"1.2.3-alpha.4"};
  constexpr version v5{"2.0.0-alpha.5"};
  constexpr version v6{"2.0.0-alpha.0"};

  ASSERT_TRUE(satisfies(v1, r1));
  ASSERT_FALSE(satisfies(v2, r1));
  ASSERT_TRUE(satisfies(v3, r1));
  ASSERT_TRUE(satisfies(v4, r1));
  ASSERT_FALSE(satisfies(v1, r2));
  ASSERT_TRUE(satisfies(v1, r3));
  ASSERT_TRUE(satisfies(v5, r4));
  ASSERT_FALSE(satisfies(v1, r4));
  ASSERT_TRUE(satisfies(v5, r5));
  ASSERT_FALSE(satisfies(v5, r6));
  ASSERT_TRUE(satisfies(v5, r7));
  ASSERT_TRUE(satisfies(v6, r7));
  ASSERT_FALSE(satisfies(v5, r8));
  ASSERT_FALSE(satisfies(v6, r8));

  ASSERT_TRUE(satisfies(v1, r1, version_range_option::include_prerelease));
  ASSERT_TRUE(satisfies(v2, r1, version_range_option::include_prerelease));
  ASSERT_TRUE(satisfies(v3, r1, version_range_option::include_prerelease));
  ASSERT_TRUE(satisfies(v4, r1, version_range_option::include_prerelease));
  ASSERT_FALSE(satisfies(v1, r2, version_range_option::include_prerelease));
  ASSERT_TRUE(satisfies(v1, r3, version_range_option::include_prerelease));
  ASSERT_TRUE(satisfies(v5, r4, version_range_option::include_prerelease));
  ASSERT_FALSE(satisfies(v1, r4, version_range_option::include_prerelease));
  ASSERT_TRUE(satisfies(v5, r5, version_range_option::include_prerelease));
  ASSERT_FALSE(satisfies(v5, r6, version_range_option::include_prerelease));
  ASSERT_TRUE(satisfies(v5, r7, version_range_option::include_prerelease));
  ASSERT_TRUE(satisfies(v6, r7, version_range_option::include_prerelease));
  ASSERT_FALSE(satisfies(v5, r8, version_range_option::include_prerelease));
  ASSERT_FALSE(satisfies(v6, r8, version_range_option::include_prerelease));
}