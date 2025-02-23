#include <rll/bit.h>
#include <catch2/catch_all.hpp>

using namespace rll;

TEST_CASE("Bit", "[bit]") {
  SECTION("Is power of two") {
    REQUIRE_FALSE(is_pow2(0));
    REQUIRE(is_pow2(1));
    REQUIRE(is_pow2(2));
    REQUIRE(is_pow2(4));
    REQUIRE_FALSE(is_pow2(5));
    REQUIRE(is_pow2(8));
    REQUIRE_FALSE(is_pow2(9));
    REQUIRE(is_pow2(16));
    REQUIRE_FALSE(is_pow2(17));
    REQUIRE(is_pow2(32));
    REQUIRE_FALSE(is_pow2(33));
    REQUIRE(is_pow2(64));
    REQUIRE_FALSE(is_pow2(65));
    REQUIRE(is_pow2(128));
    REQUIRE_FALSE(is_pow2(129));
    REQUIRE(is_pow2(256));
    REQUIRE_FALSE(is_pow2(257));
    REQUIRE(is_pow2(512));
    REQUIRE_FALSE(is_pow2(513));
    REQUIRE(is_pow2(1'024));
  }
}
