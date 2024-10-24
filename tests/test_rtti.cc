#include <rolly/rtti.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace rolly;

TEST(Rtti, Demangle)
{
  EXPECT_EQ(rtti::type_name<int>(), "int");
  EXPECT_EQ(rtti::type_name<float>(), "float");
  EXPECT_EQ(rtti::type_name<double>(), "double");
  EXPECT_EQ(rtti::type_name<char>(), "char");
  EXPECT_THAT(rtti::type_name<std::vector<int>>(), testing::AnyOf(
    "std::vector<int, std::allocator<int> >",
    "class std::vector<int,class std::allocator<int> >"
  ));
}