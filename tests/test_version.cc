#include <rolly/rolly.h>

#include <catch2/catch_all.hpp>

using namespace rolly;
using namespace std::string_view_literals;

TEST_CASE("Version", "[version]") {
  SECTION("DefaultConstructor") {
    auto constexpr v = version();

    REQUIRE(v.major == 0);
    REQUIRE(v.minor == 1);
    REQUIRE(v.patch == 0);
    REQUIRE(v.prerelease_type == prerelease::none);
    REQUIRE_FALSE(v.prerelease_number.has_value());
  }

  SECTION("Constructor") {
    auto constexpr v = version(1, 2, 3, prerelease::rc);

    REQUIRE(v.major == 1);
    REQUIRE(v.minor == 2);
    REQUIRE(v.patch == 3);
    REQUIRE(v.prerelease_type == prerelease::rc);
    REQUIRE_FALSE(v.prerelease_number.has_value());
  }

  SECTION("ConstructorWithPrerelease") {
    auto constexpr v = version(1, 2, 3, prerelease::rc, 4);

    REQUIRE(v.major == 1);
    REQUIRE(v.minor == 2);
    REQUIRE(v.patch == 3);
    REQUIRE(v.prerelease_type == prerelease::rc);
    REQUIRE(v.prerelease_number.has_value());
    REQUIRE(*v.prerelease_number == 4);
  }

  SECTION("Parse") {
    auto constexpr v = version("1.2.3-alpha.4");

    REQUIRE(v.major == 1);
    REQUIRE(v.minor == 2);
    REQUIRE(v.patch == 3);
    REQUIRE(v.prerelease_type == prerelease::alpha);
    REQUIRE(v.prerelease_number.has_value());
    REQUIRE(*v.prerelease_number == 4);
    REQUIRE(v.to_string() == "1.2.3-alpha.4");
  }

  SECTION("ParseFail") { REQUIRE_THROWS(version("1.2.3-asd")); }
}

TEST_CASE("VersionRange", "[version.range]") {
  constexpr auto r1 = ">1.2.3-alpha.3"sv;
  constexpr auto r2 = ">=1.2.3 < 2.0.0"sv;
  constexpr auto r3 = ">=1.2.3-alpha.7 <2.0.0"sv;
  constexpr auto r4 = ">1.2.3 <2.0.0-alpha.10"sv;
  constexpr auto r5 = ">1.2.3 <2.0.0-alpha.1 || <=2.0.0-alpha.5"sv;
  constexpr auto r6 = "<=2.0.0-alpha.4"sv;
  constexpr auto r7 = ">=2.0.0-alpha"sv;
  constexpr auto r8 = "<2.0.0-alpha"sv;

  constexpr version v1 {"1.2.3-alpha.7"};
  constexpr version v2 {"3.4.5-alpha.9"};
  constexpr version v3 {"3.4.5"};
  constexpr version v4 {"1.2.3-alpha.4"};
  constexpr version v5 {"2.0.0-alpha.5"};
  constexpr version v6 {"2.0.0-alpha.0"};

  SECTION("Satisfies") {
    REQUIRE(satisfies(v1, r1));
    REQUIRE(not satisfies(v2, r1));
    REQUIRE(satisfies(v3, r1));
    REQUIRE(satisfies(v4, r1));
    REQUIRE(not satisfies(v1, r2));
    REQUIRE(satisfies(v1, r3));
    REQUIRE(satisfies(v5, r4));
    REQUIRE(not satisfies(v1, r4));
    REQUIRE(satisfies(v5, r5));
    REQUIRE(not satisfies(v5, r6));
    REQUIRE(satisfies(v5, r7));
    REQUIRE(satisfies(v6, r7));
    REQUIRE(not satisfies(v5, r8));
    REQUIRE(not satisfies(v6, r8));
  }

  SECTION("Satisfies with prerelease") {
    REQUIRE(satisfies(v1, r1, version_range_option::include_prerelease));
    REQUIRE(satisfies(v2, r1, version_range_option::include_prerelease));
    REQUIRE(satisfies(v3, r1, version_range_option::include_prerelease));
    REQUIRE(satisfies(v4, r1, version_range_option::include_prerelease));
    REQUIRE(not satisfies(v1, r2, version_range_option::include_prerelease));
    REQUIRE(satisfies(v1, r3, version_range_option::include_prerelease));
    REQUIRE(satisfies(v5, r4, version_range_option::include_prerelease));
    REQUIRE(not satisfies(v1, r4, version_range_option::include_prerelease));
    REQUIRE(satisfies(v5, r5, version_range_option::include_prerelease));
    REQUIRE(not satisfies(v5, r6, version_range_option::include_prerelease));
    REQUIRE(satisfies(v5, r7, version_range_option::include_prerelease));
    REQUIRE(satisfies(v6, r7, version_range_option::include_prerelease));
    REQUIRE(not satisfies(v5, r8, version_range_option::include_prerelease));
    REQUIRE(not satisfies(v6, r8, version_range_option::include_prerelease));
  }
}
