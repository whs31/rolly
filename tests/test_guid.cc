#include <rolly/types/guid.h>

#include <sstream>
#include <catch2/catch_all.hpp>

using namespace rolly;

TEST_CASE("GUID", "[types.guid]") {
  SECTION("Random") {
    auto const r1 = guid::random();
    auto const r2 = guid::random();
    auto const r3 = guid::random();

    REQUIRE(r1 != r2);
    REQUIRE(r1 != r3);
    REQUIRE(r2 != r3);
  }

  auto const s1 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
  auto const s2 = guid("16d1bd03-09a5-47d3-944b-5e326fd52d27");
  auto const s3 = guid("fdaba646-e07e-49de-9529-4499a5580c75");
  auto const s4 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
  auto const s5 = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3b31");

  SECTION("Compare") {
    REQUIRE(s1 == s4);
    REQUIRE(s1 != s2);
    REQUIRE(s1 != s3);
    REQUIRE(s1 != s5);
  }

  SECTION("Ostream") {
    auto ss = std::stringstream();
    ss << s1;

    REQUIRE(ss.str() == "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
  }

  SECTION("ToString") {
    REQUIRE(s1.to_string() == "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
  }

  SECTION("Format") {
    REQUIRE(fmt::format("{}", s1) == "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e");
  }

  SECTION("ParseFail") {
    auto const invalid = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3baskdfmsadf3e");

    REQUIRE_FALSE(invalid.valid());
    REQUIRE_FALSE(invalid);
  }

  SECTION("Literal") {
    REQUIRE(s1 == "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e"_guid);
  }
}
