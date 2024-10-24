#include <cstdint>
#include <gtest/gtest.h>
#include <rolly/serialization.h>

using namespace rolly;

struct TestStruct
{
  int a = 1;
  int b = 2;
};

template <>
auto serialization::serialize<serialization::format::json>(TestStruct const& value) -> std::basic_string<char>
{
  return fmt::format(R"({{"a": {}, "b": {}}})", value.a, value.b);
}

template <>
auto serialization::deserialize<serialization::format::json>(std::basic_string<char> const& value) -> TestStruct
{
  return {
    .a = 1,
    .b = 2
  };
}

TEST(Serialization, Test)
{
  namespace format = rolly::serialization::format;

  auto const test = TestStruct { 1, 2 };
  auto const expected = test;
  auto json = serialization::serialize<format::json>(test);

  auto test2 = serialization::deserialize<format::json, TestStruct>(json);
  ASSERT_EQ(expected.a, test2.a);
  ASSERT_EQ(expected.b, test2.b);
  ASSERT_EQ(json, R"({"a": 1, "b": 2})");
}