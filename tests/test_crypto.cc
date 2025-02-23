#include <catch2/catch_all.hpp>
#include <rll/crypto.h>

using namespace rll;
using namespace std::string_literals;

TEST_CASE("Crypto", "[crypto]") {
  SECTION("MD5") {
    auto const test_string = "123123"s;
    auto hasher = crypto::md5();
    hasher << test_string;
    REQUIRE(hasher.hash_string() == "d3c8ce0ea199ba5b1f9f80a3f3dd5ab2");
    REQUIRE(hasher.hash_string() == "d3c8ce0ea199ba5b1f9f80a3f3dd5ab2");
    REQUIRE(hasher.hash_uuid() == "{d3c8ce0e-a199-ba5b-1f9f-80a3f3dd5ab2}"_uuid);
    hasher << 32;
    REQUIRE(hasher.hash_string() == "c917288ccbb30ecc5935710c30774e0c");
    REQUIRE(hasher.hash_string() == "c917288ccbb30ecc5935710c30774e0c");
    REQUIRE(hasher.hash_uuid() == "{c917288c-cbb3-0ecc-5935-710c30774e0c}"_uuid);
  }  // MD5
}
