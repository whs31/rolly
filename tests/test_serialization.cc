#include <rolly/serialization.h>
#include <catch2/catch_all.hpp>

using namespace rolly;

struct TestStruct {
  int a = 1;
  int b = 2;
};

template <> std::basic_string<char> serialization::serialize<serialization::format::json>(TestStruct const& value) {
  return fmt::format(R"({{"a": {}, "b": {}}})", value.a, value.b);
}

template <> TestStruct serialization::deserialize<serialization::format::json>(std::basic_string<char> const& value) {
  return { .a = 1, .b = 2 };
}

TEST_CASE("Serialization", "[serialization]") {
  namespace format = rolly::serialization::format;

  SECTION("Basic") {
    auto const test = TestStruct { 1, 2 };
    auto const expected = test;
    auto json = serialization::serialize<format::json>(test);
    auto test2 = serialization::deserialize<format::json, TestStruct>(json);

    REQUIRE(expected.a == test2.a);
    REQUIRE(expected.b == test2.b);
    REQUIRE(json == R"({"a": 1, "b": 2})");
  }
}