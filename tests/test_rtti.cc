#include <rll/rtti.h>
#include <catch2/catch_all.hpp>

using namespace rll;

TEST_CASE("Rtti", "[rtti]") {
  SECTION("Demangle") {
    REQUIRE(rtti::demangle("int") == "int");
    REQUIRE(rtti::type_name<int>() == "int");
    REQUIRE(rtti::type_name<float>() == "float");
    REQUIRE(rtti::type_name<double>() == "double");
    REQUIRE(rtti::type_name<char>() == "char");
    REQUIRE((rtti::type_name<std::vector<int>>().find("std::") != std::string::npos));
  }
}
