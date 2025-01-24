#include <rolly/types.h>

#include <sstream>
#include <type_traits>
#include <vector>
#include <string>
#include <numeric>
#include <iomanip>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace rolly;
using Catch::Matchers::WithinRel;
using Catch::Matchers::WithinAbs;

struct takes_init_and_variadic {
  std::vector<int> v;
  std::tuple<int, int> t;

  template <class... Args>
  takes_init_and_variadic(std::initializer_list<int> l, Args&&... args)
    : v(l)
    , t(std::forward<Args>(args)...) {}
};

template <typename Iter>
constexpr bool test_iterator(Iter begin, Iter end) {
  if(begin == end || *begin++ != 't')
    return false;
  if(begin == end || *begin++ != 'e')
    return false;
  if(begin == end || *begin++ != 's')
    return false;
  if(begin == end || *begin++ != 't')
    return false;
  if(begin == end || *begin++ != ' ')
    return false;
  if(begin == end || *begin++ != 'i')
    return false;
  if(begin == end || *begin++ != 't')
    return false;
  if(begin == end || *begin++ != 'e')
    return false;
  if(begin == end || *begin++ != 'r')
    return false;
  if(begin == end || *begin++ != 'a')
    return false;
  if(begin == end || *begin++ != 't')
    return false;
  if(begin == end || *begin++ != 'o')
    return false;
  if(begin == end || *begin++ != 'r')
    return false;
  if(begin != end)
    return false;
  return true;
}

template <typename Iter>
constexpr bool test_reverse_iterator(Iter begin, Iter end) {
  if(begin == end || *begin++ != 'r')
    return false;
  if(begin == end || *begin++ != 'o')
    return false;
  if(begin == end || *begin++ != 't')
    return false;
  if(begin == end || *begin++ != 'a')
    return false;
  if(begin == end || *begin++ != 'r')
    return false;
  if(begin == end || *begin++ != 'e')
    return false;
  if(begin == end || *begin++ != 't')
    return false;
  if(begin == end || *begin++ != 'i')
    return false;
  if(begin == end || *begin++ != ' ')
    return false;
  if(begin == end || *begin++ != 't')
    return false;
  if(begin == end || *begin++ != 's')
    return false;
  if(begin == end || *begin++ != 'e')
    return false;
  if(begin == end || *begin++ != 't')
    return false;
  if(begin != end)
    return false;
  return true;
}

TEST_CASE("Types", "[types]") {
  SECTION("Fixed string") {
    SECTION("Constexpr") {
      constexpr auto str_11 = make_fixed_string("test string\0\0\0");
      constexpr auto str_11_max_length = str_11.capacity();
      constexpr auto str_11_length = str_11.length;
      constexpr auto str_11_size = str_11.size();
      constexpr auto str_11_empty = str_11.empty();
      REQUIRE(str_11_max_length == 14);
      REQUIRE(str_11_length == 11);
      REQUIRE(str_11_size == 11);
      REQUIRE(not str_11_empty);

      constexpr auto str_1 = make_fixed_string("a");
      constexpr auto str_1_length = str_1.length;
      constexpr auto str_1_size = str_1.size();
      constexpr auto str_1_empty = str_1.empty();
      REQUIRE(str_1_length == 1);
      REQUIRE(str_1_size == 1);
      REQUIRE(not str_1_empty);

      constexpr auto str_2 = make_fixed_string(L"wide string");
      constexpr auto str_2_max_length = str_2.capacity();
      constexpr auto str_2_length = str_2.length;
      constexpr auto str_2_size = str_2.size();
      constexpr auto str_2_empty = str_2.empty();
      REQUIRE(str_2_max_length == 11);
      REQUIRE(str_2_length == 11);
      REQUIRE(str_2_size == 11);
      REQUIRE(not str_2_empty);
      REQUIRE(str_2[0] == 'w');
      REQUIRE(str_2[10] == 'g');

      constexpr auto str_3 = make_fixed_string(u"utf16 string");
      constexpr auto str_3_max_length = str_3.capacity();
      constexpr auto str_3_length = str_3.length;
      constexpr auto str_3_size = str_3.size();
      constexpr auto str_3_empty = str_3.empty();
      REQUIRE(str_3_max_length == 12);
      REQUIRE(str_3_length == 12);
      REQUIRE(str_3_size == 12);
      REQUIRE(not str_3_empty);
      REQUIRE(str_3[0] == 'u');
      REQUIRE(str_3[11] == 'g');

      constexpr auto str_4 = make_fixed_string(U"utf32 string");
      constexpr auto str_4_max_length = str_4.capacity();
      constexpr auto str_4_length = str_4.length;
      constexpr auto str_4_size = str_4.size();
      constexpr auto str_4_empty = str_4.empty();
      REQUIRE(str_4_max_length == 12);
      REQUIRE(str_4_length == 12);
      REQUIRE(str_4_size == 12);
      REQUIRE(not str_4_empty);
      REQUIRE(str_4[0] == 'u');
      REQUIRE(str_4[11] == 'g');

      constexpr auto str_5 = make_fixed_string(u8"utf8 string");
      constexpr auto str_5_max_length = str_5.capacity();
      constexpr auto str_5_length = str_5.length;
      constexpr auto str_5_size = str_5.size();
      constexpr auto str_5_empty = str_5.empty();
      REQUIRE(str_5_max_length == 11);
      REQUIRE(str_5_length == 11);
      REQUIRE(str_5_size == 11);
      REQUIRE(not str_5_empty);
      REQUIRE(str_5[0] == 'u');
      REQUIRE(str_5[10] == 'g');
    }

    SECTION("Constexpr Ctor Empty") {
      constexpr auto str_0 = make_fixed_string("");
      constexpr auto str_0_length = str_0.length;
      constexpr auto str_0_size = str_0.size();
      constexpr auto str_0_empty = str_0.empty();
      REQUIRE(str_0_length == 0);
      REQUIRE(str_0_size == 0);
      REQUIRE(str_0_empty);
    }

    SECTION("Constexpr Ctor Default") {
      constexpr fixed_string str_default;
      constexpr auto str_default_length = str_default.length;
      constexpr auto str_default_size = str_default.size();
      constexpr auto str_default_empty = str_default.empty();
      REQUIRE(str_default_length == 0);
      REQUIRE(str_default_size == 0);
      REQUIRE(str_default_empty);
    }

    SECTION("Constexpr Copy Ctor") {
      constexpr auto str = make_fixed_string("test string");
      constexpr auto copy_str = str;

      REQUIRE(copy_str.length == 11);
      REQUIRE(copy_str.size() == 11);
    }

    SECTION("Constexpr Iterators") {
      constexpr auto str = make_fixed_string("test iterator");

      constexpr auto actual = test_iterator(str.begin(), str.end());
      REQUIRE(actual);

      constexpr auto actual_const = test_iterator(str.cbegin(), str.cend());
      REQUIRE(actual_const);

      constexpr auto actual_reverse = test_reverse_iterator(str.rbegin(), str.rend());
      REQUIRE(actual_reverse);

      constexpr auto actual_const_reverse = test_reverse_iterator(str.crbegin(), str.crend());
      REQUIRE(actual_const_reverse);

      constexpr auto actual_ptr = test_iterator(str.data(), str.data() + str.size());
      REQUIRE(actual_ptr);
    }

    SECTION("Constexpr Empty Iterators") {
      constexpr auto str = make_fixed_string("");

      constexpr auto actual = str.begin() == str.end();
      REQUIRE(actual);

      constexpr auto actual_const = str.cbegin() == str.cend();
      REQUIRE(actual_const);

      constexpr auto actual_reverse = str.rbegin() == str.rend();
      REQUIRE(actual_reverse);

      constexpr auto actual_const_reverse = str.crbegin() == str.crend();
      REQUIRE(actual_const_reverse);

      constexpr auto actual_ptr = str.data() == str.data() + str.size();
      REQUIRE(actual_ptr);
    }

    SECTION("Constexpr At") {
      constexpr auto str = make_fixed_string("test");
      constexpr auto at0 = str.at(0);
      constexpr auto at1 = str.at(1);
      constexpr auto at2 = str.at(2);
      constexpr auto at3 = str.at(3);

      REQUIRE(at0 == 't');
      REQUIRE(at1 == 'e');
      REQUIRE(at2 == 's');
      REQUIRE(at3 == 't');

      constexpr auto c0 = str[0];
      constexpr auto c1 = str[1];
      constexpr auto c2 = str[2];
      constexpr auto c3 = str[3];

      REQUIRE(c0 == 't');
      REQUIRE(c1 == 'e');
      REQUIRE(c2 == 's');
      REQUIRE(c3 == 't');
    }

    SECTION("Constexpr Front Back") {
      constexpr auto str_3 = make_fixed_string("str");
      constexpr auto front_3 = str_3.front();
      constexpr auto back_3 = str_3.back();
      REQUIRE(front_3 == 's');
      REQUIRE(back_3 == 'r');

      constexpr auto str_1 = make_fixed_string("s");
      constexpr auto front_1 = str_1.front();
      constexpr auto back_1 = str_1.back();

      REQUIRE(front_1 == 's');
      REQUIRE(back_1 == 's');
    }

    SECTION("Constexpr Compare") {
      constexpr auto str_1 = make_fixed_string("12345\0\0\0");
      constexpr auto str_2 = make_fixed_string("12346");
      constexpr auto str_3 = make_fixed_string("");
      constexpr auto str_4 = make_fixed_string("\0\0\0");

      constexpr auto b_1 = str_1 == str_2;
      constexpr auto b_2 = str_1 != str_2;
      constexpr auto b_3 = str_1 < str_2;
      constexpr auto b_4 = str_1 > str_2;
      constexpr auto b_5 = str_1 <= str_2;
      constexpr auto b_6 = str_1 >= str_2;
      REQUIRE_FALSE(b_1);
      REQUIRE(b_2);
      REQUIRE(b_3);
      REQUIRE_FALSE(b_4);
      REQUIRE(b_5);
      REQUIRE_FALSE(b_6);

      constexpr auto b_7 = str_2 == str_1;
      constexpr auto b_8 = str_2 != str_1;
      constexpr auto b_9 = str_2 < str_1;
      constexpr auto b_10 = str_2 > str_1;
      constexpr auto b_11 = str_2 <= str_1;
      constexpr auto b_12 = str_2 >= str_1;
      REQUIRE_FALSE(b_7);
      REQUIRE(b_8);
      REQUIRE_FALSE(b_9);
      REQUIRE(b_10);
      REQUIRE_FALSE(b_11);
      REQUIRE(b_12);

      constexpr auto b_13 = str_2 == str_3;
      constexpr auto b_14 = str_2 != str_3;
      constexpr auto b_15 = str_2 < str_3;
      constexpr auto b_16 = str_2 > str_3;
      constexpr auto b_17 = str_2 <= str_3;
      constexpr auto b_18 = str_2 >= str_3;
      REQUIRE_FALSE(b_13);
      REQUIRE(b_14);
      REQUIRE_FALSE(b_15);
      REQUIRE(b_16);
      REQUIRE_FALSE(b_17);
      REQUIRE(b_18);

      constexpr auto b_19 = str_3 == str_2;
      constexpr auto b_20 = str_3 != str_2;
      constexpr auto b_21 = str_3 < str_2;
      constexpr auto b_22 = str_3 > str_2;
      constexpr auto b_23 = str_3 <= str_2;
      constexpr auto b_24 = str_3 >= str_2;
      REQUIRE_FALSE(b_19);
      REQUIRE(b_20);
      REQUIRE(b_21);
      REQUIRE_FALSE(b_22);
      REQUIRE(b_23);
      REQUIRE_FALSE(b_24);

      constexpr auto b_25 = str_3 == str_3;  // NOLINT(misc-redundant-expression)
      constexpr auto b_26 = str_3 != str_3;  // NOLINT(misc-redundant-expression)
      constexpr auto b_27 = str_3 < str_3;   // NOLINT(misc-redundant-expression)
      constexpr auto b_28 = str_3 > str_3;   // NOLINT(misc-redundant-expression)
      constexpr auto b_29 = str_3 <= str_3;  // NOLINT(misc-redundant-expression)
      constexpr auto b_30 = str_3 >= str_3;  // NOLINT(misc-redundant-expression)
      REQUIRE(b_25);
      REQUIRE_FALSE(b_26);
      REQUIRE_FALSE(b_27);
      REQUIRE_FALSE(b_28);
      REQUIRE(b_29);
      REQUIRE(b_30);

      constexpr auto b_31 = str_2 == str_2;  // NOLINT(misc-redundant-expression)
      constexpr auto b_32 = str_2 != str_2;  // NOLINT(misc-redundant-expression)
      constexpr auto b_33 = str_2 < str_2;   // NOLINT(misc-redundant-expression)
      constexpr auto b_34 = str_2 > str_2;   // NOLINT(misc-redundant-expression)
      constexpr auto b_35 = str_2 <= str_2;  // NOLINT(misc-redundant-expression)
      constexpr auto b_36 = str_2 >= str_2;  // NOLINT(misc-redundant-expression)
      REQUIRE(b_31);
      REQUIRE_FALSE(b_32);
      REQUIRE_FALSE(b_33);
      REQUIRE_FALSE(b_34);
      REQUIRE(b_35);
      REQUIRE(b_36);

      constexpr auto b_37 = str_3 == str_4;
      constexpr auto b_38 = str_3 != str_4;
      constexpr auto b_39 = str_3 < str_4;
      constexpr auto b_40 = str_3 > str_4;
      constexpr auto b_41 = str_3 <= str_4;
      constexpr auto b_42 = str_3 >= str_4;
      REQUIRE(b_37);
      REQUIRE_FALSE(b_38);
      REQUIRE_FALSE(b_39);
      REQUIRE_FALSE(b_40);
      REQUIRE(b_41);
      REQUIRE(b_42);

      constexpr auto b_43 = str_4 == str_3;
      constexpr auto b_44 = str_4 != str_3;
      constexpr auto b_45 = str_4 < str_3;
      constexpr auto b_46 = str_4 > str_3;
      constexpr auto b_47 = str_4 <= str_3;
      constexpr auto b_48 = str_4 >= str_3;
      REQUIRE(b_43);
      REQUIRE_FALSE(b_44);
      REQUIRE_FALSE(b_45);
      REQUIRE_FALSE(b_46);
      REQUIRE(b_47);
      REQUIRE(b_48);

      constexpr auto b_49 = str_4 == str_4;  // NOLINT(misc-redundant-expression)
      constexpr auto b_50 = str_4 != str_4;  // NOLINT(misc-redundant-expression)
      constexpr auto b_51 = str_4 < str_4;   // NOLINT(misc-redundant-expression)
      constexpr auto b_52 = str_4 > str_4;   // NOLINT(misc-redundant-expression)
      constexpr auto b_53 = str_4 <= str_4;  // NOLINT(misc-redundant-expression)
      constexpr auto b_54 = str_4 >= str_4;  // NOLINT(misc-redundant-expression)
      REQUIRE(b_49);
      REQUIRE_FALSE(b_50);
      REQUIRE_FALSE(b_51);
      REQUIRE_FALSE(b_52);
      REQUIRE(b_53);
      REQUIRE(b_54);
    }
  }  // Fixed string

  SECTION("U128") {
    SECTION("Constexpr") {
      constexpr u128 value1 = {1, 0};
      constexpr u128 value2 = value1 << 2;
      constexpr u128 value3 = value2 >> 1;
      constexpr u128 value4 = value3 * 4;
      constexpr u128 value5 = value4 / 3;

      REQUIRE(value5.upper() == 2);
      REQUIRE(value5.lower() == 0xAAAAAAAAAAAAAAAA);
    }

    SECTION("Constructors") {
      constexpr u128 value1;
      constexpr u128 value2 {};
      constexpr u128 value3 = 1;
      constexpr u128 value4(1, 2);
      constexpr u128 value5 {1, 2};
      constexpr u128 value6 = {1, 2};
      constexpr u128 value7 {1};
      constexpr u128 value8 = {1};
      auto value9 = (u128)457.3f;
      auto value10 = (u128)4.32e+20;

      REQUIRE(value1.upper() == 0);
      REQUIRE(value1.lower() == 0);
      REQUIRE(value2.upper() == 0);
      REQUIRE(value2.lower() == 0);
      REQUIRE(value3.upper() == 0);
      REQUIRE(value3.lower() == 1);
      REQUIRE(value4.upper() == 1);
      REQUIRE(value4.lower() == 2);
      REQUIRE(value5.upper() == 1);
      REQUIRE(value5.lower() == 2);
      REQUIRE(value6.upper() == 1);
      REQUIRE(value6.lower() == 2);
      REQUIRE(value7.upper() == 0);
      REQUIRE(value7.lower() == 1);
      REQUIRE(value8.upper() == 0);
      REQUIRE(value8.lower() == 1);
      REQUIRE(value9.upper() == 0);
      REQUIRE(value9.lower() == 457);
      REQUIRE(value10.upper() == 23);
      REQUIRE(value10.lower() == 0x6B344F2A78C00000);
    }

    SECTION("Copy") {
      constexpr u128 value = 5;

      constexpr u128 copy_ctor = value;
      u128 copy_operator;
      copy_operator = value;

      REQUIRE(copy_ctor.upper() == 0);
      REQUIRE(copy_ctor.lower() == 5);
      REQUIRE(copy_operator.upper() == 0);
      REQUIRE(copy_operator.lower() == 5);
    }

    SECTION("Move") {
      u128 value = 5;

      u128 move_ctor = std::move(value);     // NOLINT(performance-move-const-arg)
      u128 move_operator;
      move_operator = std::move(move_ctor);  // NOLINT(performance-move-const-arg)

      REQUIRE(move_operator.upper() == 0);
      REQUIRE(move_operator.lower() == 5);
    }

    SECTION("Operator bool") {
      constexpr u128 value1 = 0;
      constexpr u128 value2 = 5;

      constexpr auto actual1 = (bool)value1;
      constexpr auto actual2 = (bool)value2;

      REQUIRE_FALSE(actual1);
      REQUIRE(actual2);
    }

    SECTION("Operator T") {
      constexpr u128 value {4, 140'185'576'636'287};

      constexpr auto actual1 = (char)value;
      constexpr auto actual2 = (signed char)value;
      constexpr auto actual3 = (unsigned char)value;
      constexpr auto actual4 = (short)value;
      constexpr auto actual5 = (unsigned short)value;
      constexpr auto actual6 = (int)value;
      constexpr auto actual7 = (unsigned int)value;
      constexpr auto actual8 = (long)value;
      constexpr auto actual9 = (unsigned long)value;
      constexpr auto actual10 = (long long)value;
      constexpr auto actual11 = (unsigned long long)value;
      auto actual12 = (float)value;
      auto actual13 = (double)value;
      auto actual14 = (long double)value;

      REQUIRE(actual1 == (char)(0x7F7F7F7F7F7F));
      REQUIRE(actual2 == (signed char)(0x7F7F7F7F7F7F));
      REQUIRE(actual3 == (unsigned char)(0x7F7F7F7F7F7F));
      REQUIRE(actual4 == (short)(0x7F7F7F7F7F7F));
      REQUIRE(actual5 == (unsigned short)(0x7F7F7F7F7F7F));
      REQUIRE(actual6 == (int)(0x7F7F7F7F7F7F));
      REQUIRE(actual7 == (unsigned int)(0x7F7F7F7F7F7F));
      REQUIRE(actual8 == (long)(0x7F7F7F7F7F7F));
      REQUIRE(actual9 == (unsigned long)(0x7F7F7F7F7F7F));
      REQUIRE(actual10 == (long long)(0x7F7F7F7F7F7F));
      REQUIRE(actual11 == (unsigned long long)(0x7F7F7F7F7F7F));

      std::ostringstream str12;
      str12 << std::setprecision(4) << (actual12 * 1.0e-19);
      std::ostringstream str13;
      str13 << std::setprecision(4) << (actual13 * 1.0e-19);
      std::ostringstream str14;
      str14 << std::setprecision(4) << (actual14 * 1.0e-19);
      REQUIRE(str12.str() == "7.379");
      REQUIRE(str13.str() == "7.379");
      REQUIRE(str14.str() == "7.379");
    }

    SECTION("Arithmetic") {
      constexpr u128 value1 {4, 5};
      constexpr u128 value2 {4, 0xFFFFFFFFFFFFFFFFULL - 1};
      constexpr u128 value3 {4, 0xFFFFFFFFFFFFFFFFULL};

      constexpr auto plus1 = +value1;
      constexpr auto plus2 = +value2;
      constexpr auto plus3 = +value3;
      REQUIRE(plus1.upper() == 4);
      REQUIRE(plus1.lower() == 5);
      REQUIRE(plus2.upper() == 4);
      REQUIRE(plus2.lower() == 0xFFFFFFFFFFFFFFFFULL - 1);
      REQUIRE(plus3.upper() == 4);
      REQUIRE(plus3.lower() == 0xFFFFFFFFFFFFFFFFULL);

      constexpr auto minus1 = -value1;
      constexpr auto minus2 = -value2;
      constexpr auto minus3 = -value3;
      REQUIRE(minus1.upper() == 0xFFFFFFFFFFFFFFFFULL - 4);
      REQUIRE(minus1.lower() == 0xFFFFFFFFFFFFFFFFULL - 4);
      REQUIRE(minus2.upper() == 0xFFFFFFFFFFFFFFFFULL - 4);
      REQUIRE(minus2.lower() == 2);
      REQUIRE(minus3.upper() == 0xFFFFFFFFFFFFFFFFULL - 4);
      REQUIRE(minus3.lower() == 1);

      constexpr auto inv1 = ~value1;
      constexpr auto inv2 = ~value2;
      constexpr auto inv3 = ~value3;
      REQUIRE(inv1.upper() == 0xFFFFFFFFFFFFFFFFULL - 4);
      REQUIRE(inv1.lower() == 0xFFFFFFFFFFFFFFFFULL - 5);
      REQUIRE(inv2.upper() == 0xFFFFFFFFFFFFFFFFULL - 4);
      REQUIRE(inv2.lower() == 1);
      REQUIRE(inv3.upper() == 0xFFFFFFFFFFFFFFFFULL - 4);
      REQUIRE(inv3.lower() == 0);

      constexpr auto sum1 = value1 + 1;
      constexpr auto sum2 = value2 + 1;
      constexpr auto sum3 = value3 + 1;
      constexpr auto sum4 = value1 + u128(10, 0xFFFFFFFFFFFFFFFFULL - 2);
      constexpr auto sum5 = 1 + value1;
      REQUIRE(sum1.upper() == 4);
      REQUIRE(sum1.lower() == 6);
      REQUIRE(sum2.upper() == 4);
      REQUIRE(sum2.lower() == 0xFFFFFFFFFFFFFFFFULL);
      REQUIRE(sum3.upper() == 5);
      REQUIRE(sum3.lower() == 0);
      REQUIRE(sum4.upper() == 15);
      REQUIRE(sum4.lower() == 2);
      REQUIRE(sum5.upper() == 4);
      REQUIRE(sum5.lower() == 6);

      constexpr auto sub1 = value1 - 6;
      constexpr auto sub2 = value2 - 7;
      constexpr auto sub3 = value3 - 1;
      constexpr auto sub4 = value1 - u128(2, 0xFFFFFFFFFFFFFFFFULL - 2);
      constexpr auto sub5 = 10 - u128(7);
      REQUIRE(sub1.upper() == 3);
      REQUIRE(sub1.lower() == 0xFFFFFFFFFFFFFFFFULL);
      REQUIRE(sub2.upper() == 4);
      REQUIRE(sub2.lower() == 0xFFFFFFFFFFFFFFFFULL - 8);
      REQUIRE(sub3.upper() == 4);
      REQUIRE(sub3.lower() == 0xFFFFFFFFFFFFFFFFULL - 1);
      REQUIRE(sub4.upper() == 1);
      REQUIRE(sub4.lower() == 8);
      REQUIRE(sub5.upper() == 0);
      REQUIRE(sub5.lower() == 3);

      constexpr auto mul1 = value1 * 6;
      constexpr auto mul2 = value2 * 7;
      constexpr auto mul3 = value3 * 1;
      constexpr auto mul4 = 1'000 * u128(2, 0xFFFFFFFFFFFFFFFFULL - 2);
      REQUIRE(mul1.upper() == 24);
      REQUIRE(mul1.lower() == 30);
      REQUIRE(mul2.upper() == 34);
      REQUIRE(mul2.lower() == 0XFFFFFFFFFFFFFFF2ULL);
      REQUIRE(mul3.upper() == 4);
      REQUIRE(mul3.lower() == 0xFFFFFFFFFFFFFFFFULL);
      REQUIRE(mul4.upper() == 0xBB7);
      REQUIRE(mul4.lower() == 0xFFFFFFFFFFFFF448);

      auto div1 = value1 / 25;
      auto div2 = value2 / 2;
      auto div3 = value3 / 1;
      auto div4 = 1'000 / u128(0, 2);
      auto div5 = 0 / u128(0, 2);
      auto div6 = 0 / u128(0, 0);
      auto div7 = 1'000 / u128(0, 1'000);
      auto div8 = 1'000 / u128(0, 2'000);
      auto div9 = u128(2, 1'000) / u128(2, 1'000);
      auto div10 = u128(2, 1'000) / u128(2, 2'000);
      REQUIRE(div1.upper() == 0);
      REQUIRE(div1.lower() == 0x28F5C28F5C28F5C2);
      REQUIRE(div2.upper() == 2);
      REQUIRE(div2.lower() == 0x7fffffffffffffffULL);
      REQUIRE(div3.upper() == 4);
      REQUIRE(div3.lower() == 0xFFFFFFFFFFFFFFFFULL);
      REQUIRE(div4.upper() == 0);
      REQUIRE(div4.lower() == 0x1F4);
      REQUIRE(div5.upper() == 0);
      REQUIRE(div5.lower() == 0);
      REQUIRE(div6.upper() == 0);
      REQUIRE(div6.lower() == 0);
      REQUIRE(div7.upper() == 0);
      REQUIRE(div7.lower() == 1);
      REQUIRE(div8.upper() == 0);
      REQUIRE(div8.lower() == 0);
      REQUIRE(div9.upper() == 0);
      REQUIRE(div9.lower() == 1);
      REQUIRE(div10.upper() == 0);
      REQUIRE(div10.lower() == 0);

      auto rem1 = value1 % 25;
      auto rem2 = value2 % 2;
      auto rem3 = value3 % 1;
      auto rem4 = 15 % value1;
      REQUIRE(rem1.upper() == 0);
      REQUIRE(rem1.lower() == 19);
      REQUIRE(rem2.upper() == 0);
      REQUIRE(rem2.lower() == 0);
      REQUIRE(rem3.upper() == 0);
      REQUIRE(rem3.lower() == 0);
      REQUIRE(rem4.upper() == 0);
      REQUIRE(rem4.lower() == 15);

      auto and1 = u128(1, 1) & u128(3, 3);
      auto and2 = u128(1, 1) & 1;
      auto and3 = 1 & u128(1, 1);
      REQUIRE(and1.upper() == 1);
      REQUIRE(and1.lower() == 1);
      REQUIRE(and2.upper() == 0);
      REQUIRE(and2.lower() == 1);
      REQUIRE(and3.upper() == 0);
      REQUIRE(and3.lower() == 1);

      auto or1 = u128(1, 1) | u128(2, 2);
      auto or2 = u128(1, 1) | 2;
      auto or3 = 2 | u128(1, 1);
      REQUIRE(or1.upper() == 3);
      REQUIRE(or1.lower() == 3);
      REQUIRE(or2.upper() == 1);
      REQUIRE(or2.lower() == 3);
      REQUIRE(or3.upper() == 1);
      REQUIRE(or3.lower() == 3);

      auto xor1 = u128(1, 1) ^ u128(3, 3);
      auto xor2 = u128(1, 1) ^ 3;
      auto xor3 = 3 ^ u128(1, 1);
      REQUIRE(xor1.upper() == 2);
      REQUIRE(xor1.lower() == 2);
      REQUIRE(xor2.upper() == 1);
      REQUIRE(xor2.lower() == 2);
      REQUIRE(xor3.upper() == 1);
      REQUIRE(xor3.lower() == 2);

      auto lshift1 = u128(0, 0xFFFFFFFFFFFFFFFFULL) << 1;
      REQUIRE(lshift1.upper() == 1);
      REQUIRE(lshift1.lower() == 0xFFFFFFFFFFFFFFFEULL);
    }

    SECTION("Assignment") {
      u128 value {4, 5};

      value += 3;
      REQUIRE(value.upper() == 4);
      REQUIRE(value.lower() == 8);

      value -= 2;
      REQUIRE(value.upper() == 4);
      REQUIRE(value.lower() == 6);

      value *= 2;
      REQUIRE(value.upper() == 8);
      REQUIRE(value.lower() == 12);

      value /= 2;
      REQUIRE(value.upper() == 4);
      REQUIRE(value.lower() == 6);

      value %= 3;
      REQUIRE(value.upper() == 0);
      REQUIRE(value.lower() == 1);

      value &= 0;
      REQUIRE(value.upper() == 0);
      REQUIRE(value.lower() == 0);

      value |= 1;
      REQUIRE(value.upper() == 0);
      REQUIRE(value.lower() == 1);

      value ^= 3;
      REQUIRE(value.upper() == 0);
      REQUIRE(value.lower() == 2);

      value <<= 1;
      REQUIRE(value.upper() == 0);
      REQUIRE(value.lower() == 4);

      value >>= 1;
      REQUIRE(value.upper() == 0);
      REQUIRE(value.lower() == 2);
    }

    SECTION("IncDec") {
      u128 value1 {4, 5};
      u128 value2 {4, 0xFFFFFFFFFFFFFFFFULL - 1};
      u128 value3 {4, 0xFFFFFFFFFFFFFFFFULL};

      REQUIRE(++value1 == u128(4, 6));
      REQUIRE(++value2 == u128(4, 0xFFFFFFFFFFFFFFFFULL));
      REQUIRE(++value3 == u128(5, 0));
      REQUIRE(value1++ == u128(4, 6));
      REQUIRE(value2++ == u128(4, 0xFFFFFFFFFFFFFFFFULL));
      REQUIRE(value3++ == u128(5, 0));
      REQUIRE(value1 == u128(4, 7));
      REQUIRE(value2 == u128(5, 0));
      REQUIRE(value3 == u128(5, 1));

      REQUIRE(value1-- == u128(4, 7));
      REQUIRE(value2-- == u128(5, 0));
      REQUIRE(value3-- == u128(5, 1));
      REQUIRE(value1 == u128(4, 6));
      REQUIRE(value2 == u128(4, 0xFFFFFFFFFFFFFFFFULL));
      REQUIRE(value3 == u128(5, 0));
      REQUIRE(--value1 == u128(4, 5));
      REQUIRE(--value2 == u128(4, 0xFFFFFFFFFFFFFFFFULL - 1));
      REQUIRE(--value3 == u128(4, 0xFFFFFFFFFFFFFFFFULL));
    }

    SECTION("Logical") {
      u128 const value1 {0, 0};
      u128 const value2 {0, 1};
      u128 const value3 {1, 0};
      u128 const value4 {1, 1};

      REQUIRE_FALSE((bool)value1);
      REQUIRE((bool)value2);
      REQUIRE((bool)value3);
      REQUIRE((bool)value4);

      REQUIRE(! value1);
      REQUIRE_FALSE(! value2);
      REQUIRE_FALSE(! value3);
      REQUIRE_FALSE(! value4);

      REQUIRE_FALSE((value1 && value1));
      REQUIRE_FALSE((value1 && value2));
      REQUIRE((value2 && value3));
      REQUIRE((value3 && value4));
      REQUIRE_FALSE((value4 && value1));
      REQUIRE((value4 && value4));

      REQUIRE_FALSE((value1 || value1));
      REQUIRE((value1 || value2));
      REQUIRE((value2 || value3));
      REQUIRE((value3 || value4));
      REQUIRE((value4 || value1));
      REQUIRE((value4 || value4));
    }

    SECTION("Comparison") {
      u128 const value1 {0, 0};
      u128 const value2 {0, 1};
      u128 const value3 {1, 0};
      u128 const value4 {1, 1};

      REQUIRE_FALSE((value1 == value2));
      REQUIRE((value1 != value2));
      REQUIRE((value1 < value2));
      REQUIRE_FALSE((value1 > value2));
      REQUIRE((value1 <= value2));
      REQUIRE_FALSE((value1 >= value2));

      REQUIRE_FALSE((value1 == value3));
      REQUIRE((value1 != value3));
      REQUIRE((value1 < value3));
      REQUIRE_FALSE((value1 > value3));
      REQUIRE((value1 <= value3));
      REQUIRE_FALSE((value1 >= value3));

      REQUIRE((value2 == 1));
      REQUIRE_FALSE((value2 != 1));
      REQUIRE_FALSE((value2 < 1));
      REQUIRE_FALSE((value2 > 1));
      REQUIRE((value2 <= 1));
      REQUIRE((value2 >= 1));

      REQUIRE((1 == value2));
      REQUIRE_FALSE((1 != value2));
      REQUIRE_FALSE((1 < value2));
      REQUIRE_FALSE((1 > value2));
      REQUIRE((1 <= value2));
      REQUIRE((1 >= value2));

      REQUIRE((value3 == value3));
      REQUIRE_FALSE((value3 != value3));
      REQUIRE_FALSE((value3 < value3));
      REQUIRE_FALSE((value3 > value3));
      REQUIRE((value3 <= value3));
      REQUIRE((value3 >= value3));

      REQUIRE_FALSE((value4 == value2));
      REQUIRE((value4 != value2));
      REQUIRE_FALSE((value4 < value2));
      REQUIRE((value4 > value2));
      REQUIRE_FALSE((value4 <= value2));
      REQUIRE((value4 >= value2));
    }

    SECTION("Swap") {
      u128 value1 = 0;
      u128 value2 = {4, 5};

      std::swap(value1, value2);

      REQUIRE(value1.upper() == 4);
      REQUIRE(value1.lower() == 5);
      REQUIRE(value2.upper() == 0);
      REQUIRE(value2.lower() == 0);
    }

    SECTION("ToString") {
      u128 value1 = 17'852;
      u128 value2 = {4, 17'852};

      std::ostringstream ss1;
      ss1 << value1;
      std::ostringstream ss2;
      ss2 << std::dec << value1;
      std::ostringstream ss3;
      ss3 << std::oct << value1;
      std::ostringstream ss4;
      ss4 << std::hex << value1;
      std::ostringstream ss5;
      ss5 << std::hex << std::uppercase << value1;
      std::ostringstream ss6;
      ss6 << value2;
      std::ostringstream ss7;
      ss7 << std::dec << value2;
      std::ostringstream ss8;
      ss8 << std::oct << value2;
      std::ostringstream ss9;
      ss9 << std::hex << value2;
      std::ostringstream ss10;
      ss10 << std::hex << std::uppercase << value2;

      REQUIRE(ss1.str() == std::string("17852"));
      REQUIRE(ss2.str() == std::string("17852"));
      REQUIRE(ss3.str() == std::string("42674"));
      REQUIRE(ss4.str() == std::string("45bc"));
      REQUIRE(ss5.str() == std::string("45BC"));
      REQUIRE(ss6.str() == std::string("73786976294838224316"));
      REQUIRE(ss7.str() == std::string("73786976294838224316"));
      REQUIRE(ss8.str() == std::string("10000000000000000042674"));
      REQUIRE(ss9.str() == std::string("400000000000045bc"));
      REQUIRE(ss10.str() == std::string("400000000000045BC"));
    }

    SECTION("FromString") {
      u128 const expected1 = 17'852;
      u128 const expected2 = {4, 17'852};

      // NOLINTNEXTLINE(readability-isolate-declaration)
      u128 read1, read2, read3, read4, read5, read6, read7, read8, read9, read10, read11;
      std::istringstream ss1("17852 test");
      ss1 >> read1;
      std::istringstream ss2("17852 test");
      ss2 >> std::dec >> read2;
      std::istringstream ss3("42674 test");
      ss3 >> std::oct >> read3;
      std::istringstream ss4("45bc test");
      ss4 >> std::hex >> read4;
      std::istringstream ss5("45BC test");
      ss5 >> std::hex >> read5;
      std::istringstream ss6("73786976294838224316 test");
      ss6 >> read6;
      std::istringstream ss7("73786976294838224316 test");
      ss7 >> std::dec >> read7;
      std::istringstream ss8("10000000000000000042674 test");
      ss8 >> std::oct >> read8;
      std::istringstream ss9("400000000000045bc test");
      ss9 >> std::hex >> read9;
      std::istringstream ss10("400000000000045BC test");
      ss10 >> std::hex >> read10;
      std::istringstream ss11("bad");
      ss11 >> read11;

      REQUIRE(read1 == expected1);
      REQUIRE(read2 == expected1);
      REQUIRE(read3 == expected1);
      REQUIRE(read4 == expected1);
      REQUIRE(read5 == expected1);
      REQUIRE(read6 == expected2);
      REQUIRE(read7 == expected2);
      REQUIRE(read8 == expected2);
      REQUIRE(read9 == expected2);
      REQUIRE(read10 == expected2);
      REQUIRE(ss11.fail());
      REQUIRE(read11 == u128(0));
    }

    SECTION("NumericLimits") {
      REQUIRE(std::numeric_limits<u128>::is_integer);
      REQUIRE(std::numeric_limits<u128>::digits == 128);
      REQUIRE(std::numeric_limits<u128>::lowest() == u128());
      REQUIRE(std::numeric_limits<u128>::min() == u128());
      REQUIRE(std::numeric_limits<u128>::max() == u128(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF));
    }

    SECTION("Abs") {
      constexpr auto actual1 = std::abs(u128(5));

      REQUIRE(actual1 == u128(5));
    }
  }  // U128

  SECTION("GUID", "[types.guid]") {
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

    SECTION("ToString") { REQUIRE(s1.to_string() == "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e"); }

    SECTION("Format") { REQUIRE(fmt::format("{}", s1) == "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e"); }

    SECTION("ParseFail") { REQUIRE_THROWS(guid("7bcd757f-5b10-4f9b-af69-1a1f226f3baskdfmsadf3e")); }

    SECTION("Literal") { REQUIRE(s1 == "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e"_guid); }

    SECTION("Constexpr") {
      auto constexpr u = "{7bcd757f-5b10-4f9b-af69-1a1f226f3b3e}"_guid;

#ifdef ___rolly_cxx20___
      STATIC_REQUIRE(u.valid());
      STATIC_REQUIRE(u == "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e"_guid);
#endif
    }
  }  // GUID

  SECTION("Result", "[types.result]") {
    SECTION("Assignment") {
      expected<int, int> e1 = 42;
      expected<int, int> e2 = 17;
      expected<int, int> e3 = 21;
      expected<int, int> e4 = make_unexpected(42);
      expected<int, int> e5 = make_unexpected(17);
      expected<int, int> e6 = make_unexpected(21);

      e1 = e2;
      REQUIRE(e1);
      REQUIRE(*e1 == 17);
      REQUIRE(e2);
      REQUIRE(*e2 == 17);

      e1 = std::move(e2);
      REQUIRE(e1);
      REQUIRE(*e1 == 17);
      REQUIRE(e2);
      REQUIRE(*e2 == 17);

      e1 = 42;
      REQUIRE(e1);
      REQUIRE(*e1 == 42);

      auto unex = make_unexpected(12);
      e1 = unex;
      REQUIRE(! e1);
      REQUIRE(e1.error() == 12);

      e1 = make_unexpected(42);
      REQUIRE(! e1);
      REQUIRE(e1.error() == 42);

      e1 = e3;
      REQUIRE(e1);
      REQUIRE(*e1 == 21);

      e4 = e5;
      REQUIRE(! e4);
      REQUIRE(e4.error() == 17);

      e4 = std::move(e6);
      REQUIRE(! e4);
      REQUIRE(e4.error() == 21);
      REQUIRE_THROWS(e4.unwrap<std::runtime_error>());

      e4 = e1;
      REQUIRE(e4);
      REQUIRE(*e4 == 21);
    }

    SECTION("Triviality") {
      REQUIRE(std::is_trivially_copy_constructible<expected<int, int>>::value);
      REQUIRE(std::is_trivially_copy_assignable<expected<int, int>>::value);
      REQUIRE(std::is_trivially_move_constructible<expected<int, int>>::value);
      REQUIRE(std::is_trivially_move_assignable<expected<int, int>>::value);
      REQUIRE(std::is_trivially_destructible<expected<int, int>>::value);

      REQUIRE(std::is_trivially_copy_constructible<expected<void, int>>::value);
      REQUIRE(std::is_trivially_move_constructible<expected<void, int>>::value);
      REQUIRE(std::is_trivially_destructible<expected<void, int>>::value);

      SECTION("Triviality of defaulted struct") {
        struct T {
          T(T const&) = default;
          T(T&&) = default;
          T& operator=(T const&) = default;
          T& operator=(T&&) = default;
          ~T() = default;
        };

        REQUIRE(std::is_trivially_copy_constructible<expected<T, int>>::value);
        REQUIRE(std::is_trivially_copy_assignable<expected<T, int>>::value);
        REQUIRE(std::is_trivially_move_constructible<expected<T, int>>::value);
        REQUIRE(std::is_trivially_move_assignable<expected<T, int>>::value);
        REQUIRE(std::is_trivially_destructible<expected<T, int>>::value);
      }

      SECTION("Triviality of non-defaulted struct") {
        struct T {
          T(T const&) {}

          T(T&&) {}

          T& operator=(T const&) { return *this; }

          T& operator=(T&&) { return *this; }

          ~T() {}
        };

        REQUIRE(not std::is_trivially_copy_constructible<expected<T, int>>::value);
        REQUIRE(not std::is_trivially_copy_assignable<expected<T, int>>::value);
        REQUIRE(not std::is_trivially_move_constructible<expected<T, int>>::value);
        REQUIRE(not std::is_trivially_move_assignable<expected<T, int>>::value);
        REQUIRE(not std::is_trivially_destructible<expected<T, int>>::value);
      }
    }

    SECTION("Deletion") {
      REQUIRE(std::is_copy_constructible<expected<int, int>>::value);
      REQUIRE(std::is_copy_assignable<expected<int, int>>::value);
      REQUIRE(std::is_move_constructible<expected<int, int>>::value);
      REQUIRE(std::is_move_assignable<expected<int, int>>::value);
      REQUIRE(std::is_destructible<expected<int, int>>::value);

      {
        struct T {
          T() = default;
        };

        REQUIRE(std::is_default_constructible<expected<T, int>>::value);
      }

      {
        struct T {
          T(int) {}
        };

        REQUIRE(! std::is_default_constructible<expected<T, int>>::value);
      }

      {
        struct T {
          T(T const&) = default;
          T(T&&) = default;
          T& operator=(T const&) = default;
          T& operator=(T&&) = default;
          ~T() = default;
        };

        REQUIRE(std::is_copy_constructible<expected<T, int>>::value);
        REQUIRE(std::is_copy_assignable<expected<T, int>>::value);
        REQUIRE(std::is_move_constructible<expected<T, int>>::value);
        REQUIRE(std::is_move_assignable<expected<T, int>>::value);
        REQUIRE(std::is_destructible<expected<T, int>>::value);
      }

      {
        struct T {
          T(T const&) = delete;
          T(T&&) = delete;
          T& operator=(T const&) = delete;
          T& operator=(T&&) = delete;
        };

        REQUIRE(! std::is_copy_constructible<expected<T, int>>::value);
        REQUIRE(! std::is_copy_assignable<expected<T, int>>::value);
        REQUIRE(! std::is_move_constructible<expected<T, int>>::value);
        REQUIRE(! std::is_move_assignable<expected<T, int>>::value);
      }

      {
        struct T {
          T(T const&) = delete;
          T(T&&) = default;
          T& operator=(T const&) = delete;
          T& operator=(T&&) = default;
        };

        REQUIRE(! std::is_copy_constructible<expected<T, int>>::value);
        REQUIRE(! std::is_copy_assignable<expected<T, int>>::value);
        REQUIRE(std::is_move_constructible<expected<T, int>>::value);
        REQUIRE(std::is_move_assignable<expected<T, int>>::value);
      }

      {
        struct T {
          T(T const&) = default;
          T(T&&) = delete;
          T& operator=(T const&) = default;
          T& operator=(T&&) = delete;
        };

        REQUIRE(std::is_copy_constructible<expected<T, int>>::value);
        REQUIRE(std::is_copy_assignable<expected<T, int>>::value);
      }

      {
        expected<int, int> e;
        REQUIRE(std::is_default_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_constructible<decltype(e)>::value);
        REQUIRE(std::is_move_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_assignable<decltype(e)>::value);
        REQUIRE(std::is_move_assignable<decltype(e)>::value);
      }

      {
        expected<int, std::string> e;
        REQUIRE(std::is_default_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_constructible<decltype(e)>::value);
        REQUIRE(std::is_move_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_assignable<decltype(e)>::value);
        REQUIRE(std::is_move_assignable<decltype(e)>::value);
      }

      {
        expected<std::string, int> e;
        REQUIRE(std::is_default_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_constructible<decltype(e)>::value);
        REQUIRE(std::is_move_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_assignable<decltype(e)>::value);
        REQUIRE(std::is_move_assignable<decltype(e)>::value);
      }

      {
        expected<std::string, std::string> e;
        REQUIRE(std::is_default_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_constructible<decltype(e)>::value);
        REQUIRE(std::is_move_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_assignable<decltype(e)>::value);
        REQUIRE(std::is_move_assignable<decltype(e)>::value);
      }
    }

    SECTION("Constructors") {
      {
        expected<int, int> e;
        REQUIRE(e);
        REQUIRE(e == 0);
      }

      {
        expected<int, int> e = make_unexpected(0);
        REQUIRE(! e);
        REQUIRE(e.error() == 0);
      }

      {
        expected<int, int> e(unexpect, 0);
        REQUIRE(! e);
        REQUIRE(e.error() == 0);
      }

      {
        expected<int, int> e(std::in_place, 42);
        REQUIRE(e);
        REQUIRE(e == 42);
      }

      {
        expected<std::vector<int>, int> e(std::in_place, {0, 1});
        REQUIRE(e);
        REQUIRE((*e)[0] == 0);
        REQUIRE((*e)[1] == 1);
      }

      {
        expected<std::tuple<int, int>, int> e(std::in_place, 0, 1);
        REQUIRE(e);
        REQUIRE(std::get<0>(*e) == 0);
        REQUIRE(std::get<1>(*e) == 1);
      }

      {
        expected<takes_init_and_variadic, int> e(std::in_place, {0, 1}, 2, 3);
        REQUIRE(e);
        REQUIRE(e->v[0] == 0);
        REQUIRE(e->v[1] == 1);
        REQUIRE(std::get<0>(e->t) == 2);
        REQUIRE(std::get<1>(e->t) == 3);
      }

      {
        expected<int, int> e;
        REQUIRE(std::is_default_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_constructible<decltype(e)>::value);
        REQUIRE(std::is_move_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_assignable<decltype(e)>::value);
        REQUIRE(std::is_move_assignable<decltype(e)>::value);
      }

      {
        expected<int, std::string> e;
        REQUIRE(std::is_default_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_constructible<decltype(e)>::value);
        REQUIRE(std::is_move_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_assignable<decltype(e)>::value);
        REQUIRE(std::is_move_assignable<decltype(e)>::value);
      }

      {
        expected<std::string, int> e;
        REQUIRE(std::is_default_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_constructible<decltype(e)>::value);
        REQUIRE(std::is_move_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_assignable<decltype(e)>::value);
        REQUIRE(std::is_move_assignable<decltype(e)>::value);
      }

      {
        expected<std::string, std::string> e;
        REQUIRE(std::is_default_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_constructible<decltype(e)>::value);
        REQUIRE(std::is_move_constructible<decltype(e)>::value);
        REQUIRE(std::is_copy_assignable<decltype(e)>::value);
        REQUIRE(std::is_move_assignable<decltype(e)>::value);
      }

      {
        expected<void, int> e;
        REQUIRE(e);
      }

      {
        expected<void, int> e(unexpect, 42);
        REQUIRE(! e);
        REQUIRE(e.error() == 42);
      }
    }

    SECTION("Emplace") {
      {
        expected<std::unique_ptr<int>, int> e;
        e.emplace(new int {42});
        REQUIRE(e);
        REQUIRE(**e == 42);
      }

      {
        expected<std::vector<int>, int> e;
        e.emplace({0, 1});
        REQUIRE(e);
        REQUIRE((*e)[0] == 0);
        REQUIRE((*e)[1] == 1);
      }

      {
        expected<std::tuple<int, int>, int> e;
        e.emplace(2, 3);
        REQUIRE(e);
        REQUIRE(std::get<0>(*e) == 2);
        REQUIRE(std::get<1>(*e) == 3);
      }

      {
        expected<takes_init_and_variadic, int> e = make_unexpected(0);
        e.emplace({0, 1}, 2, 3);
        REQUIRE(e);
        REQUIRE(e->v[0] == 0);
        REQUIRE(e->v[1] == 1);
        REQUIRE(std::get<0>(e->t) == 2);
        REQUIRE(std::get<1>(e->t) == 3);
      }
    }

    SECTION("Functional extensions") {
      SECTION("Map") {
        auto mul2 = [](int a) { return a * 2; };
        auto ret_void = [](int a) { (void)a; };

        {
          expected<int, int> e = 21;
          auto ret = e.map(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> const e = 21;
          auto ret = e.map(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> e = 21;
          auto ret = std::move(e).map(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> const e = 21;
          auto ret = std::move(e).map(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.map(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.map(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).map(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).map(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> e = 21;
          auto ret = e.map(ret_void);
          REQUIRE(ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> const e = 21;
          auto ret = e.map(ret_void);
          REQUIRE(ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> e = 21;
          auto ret = std::move(e).map(ret_void);
          REQUIRE(ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> const e = 21;
          auto ret = std::move(e).map(ret_void);
          REQUIRE(ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.map(ret_void);
          REQUIRE(! ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.map(ret_void);
          REQUIRE(! ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).map(ret_void);
          REQUIRE(! ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).map(ret_void);
          REQUIRE(! ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        // mapping functions which return references
        {
          expected<int, int> e(42);
          auto ret = e.map([](int& i) -> int& { return i; });
          REQUIRE(ret);
          REQUIRE(ret == 42);
        }
      }

      SECTION("Map error") {
        auto mul2 = [](int a) { return a * 2; };
        auto ret_void = [](int a) { (void)a; };

        {
          expected<int, int> e = 21;
          auto ret = e.map_error(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> const e = 21;
          auto ret = e.map_error(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> e = 21;
          auto ret = std::move(e).map_error(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> const e = 21;
          auto ret = std::move(e).map_error(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.map_error(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 42);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.map_error(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 42);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).map_error(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 42);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).map_error(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 42);
        }

        {
          expected<int, int> e = 21;
          auto ret = e.map_error(ret_void);
          REQUIRE(ret);
        }

        {
          expected<int, int> const e = 21;
          auto ret = e.map_error(ret_void);
          REQUIRE(ret);
        }

        {
          expected<int, int> e = 21;
          auto ret = std::move(e).map_error(ret_void);
          REQUIRE(ret);
        }

        {
          expected<int, int> const e = 21;
          auto ret = std::move(e).map_error(ret_void);
          REQUIRE(ret);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.map_error(ret_void);
          REQUIRE(! ret);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.map_error(ret_void);
          REQUIRE(! ret);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).map_error(ret_void);
          REQUIRE(! ret);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).map_error(ret_void);
          REQUIRE(! ret);
        }
      }

      SECTION("And then") {
        auto succeed = [](int a) {
          (void)a;
          return expected<int, int>(21 * 2);
        };
        auto fail = [](int a) {
          (void)a;
          return expected<int, int>(unexpect, 17);
        };

        {
          expected<int, int> e = 21;
          auto ret = e.and_then(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> const e = 21;
          auto ret = e.and_then(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> e = 21;
          auto ret = std::move(e).and_then(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> const e = 21;
          auto ret = std::move(e).and_then(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> e = 21;
          auto ret = e.and_then(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 17);
        }

        {
          expected<int, int> const e = 21;
          auto ret = e.and_then(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 17);
        }

        {
          expected<int, int> e = 21;
          auto ret = std::move(e).and_then(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 17);
        }

        {
          expected<int, int> const e = 21;
          auto ret = std::move(e).and_then(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 17);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.and_then(succeed);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.and_then(succeed);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).and_then(succeed);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).and_then(succeed);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.and_then(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.and_then(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).and_then(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).and_then(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }
      }

      SECTION("Or else") {
        using eptr = std::unique_ptr<int>;
        auto succeed = [](int a) {
          (void)a;
          return expected<int, int>(21 * 2);
        };
        auto succeedptr = [](eptr e) {
          (void)e;
          return expected<int, eptr>(21 * 2);
        };
        auto fail = [](int a) {
          (void)a;
          return expected<int, int>(unexpect, 17);
        };
        auto failptr = [](eptr e) {
          *e = 17;
          return expected<int, eptr>(unexpect, std::move(e));
        };
        auto failvoid = [](int) {};
        auto failvoidptr = [](eptr const&) { /* don't consume */ };
        auto consumeptr = [](eptr) {};
        auto make_u_int = [](int n) { return std::unique_ptr<int>(new int(n)); };

        {
          expected<int, int> e = 21;
          auto ret = e.or_else(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> const e = 21;
          auto ret = e.or_else(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> e = 21;
          auto ret = std::move(e).or_else(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, eptr> e = 21;
          auto ret = std::move(e).or_else(succeedptr);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> const e = 21;
          auto ret = std::move(e).or_else(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> e = 21;
          auto ret = e.or_else(fail);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> const e = 21;
          auto ret = e.or_else(fail);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> e = 21;
          auto ret = std::move(e).or_else(fail);
          REQUIRE(ret);
          REQUIRE(ret == 21);
        }

        {
          expected<int, eptr> e = 21;
          auto ret = std::move(e).or_else(failptr);
          REQUIRE(ret);
          REQUIRE(ret == 21);
        }

        {
          expected<int, int> const e = 21;
          auto ret = std::move(e).or_else(fail);
          REQUIRE(ret);
          REQUIRE(*ret == 21);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.or_else(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.or_else(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).or_else(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, eptr> e(unexpect, make_u_int(21));
          auto ret = std::move(e).or_else(succeedptr);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).or_else(succeed);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.or_else(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 17);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.or_else(failvoid);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.or_else(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 17);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.or_else(failvoid);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).or_else(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 17);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).or_else(failvoid);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, eptr> e(unexpect, make_u_int(21));
          auto ret = std::move(e).or_else(failvoidptr);
          REQUIRE(! ret);
          REQUIRE(*ret.error() == 21);
        }

        {
          expected<int, eptr> e(unexpect, make_u_int(21));
          auto ret = std::move(e).or_else(consumeptr);
          REQUIRE(! ret);
          REQUIRE(ret.error() == nullptr);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).or_else(fail);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 17);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).or_else(failvoid);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }
      }

      SECTION("Transform extensions", "[extensions.transform]") {
        auto mul2 = [](int a) { return a * 2; };
        auto ret_void = [](int a) { (void)a; };

        {
          expected<int, int> e = 21;
          auto ret = e.transform(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> const e = 21;
          auto ret = e.transform(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> e = 21;
          auto ret = std::move(e).transform(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> const e = 21;
          auto ret = std::move(e).transform(mul2);
          REQUIRE(ret);
          REQUIRE(*ret == 42);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.transform(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.transform(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).transform(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).transform(mul2);
          REQUIRE(! ret);
          REQUIRE(ret.error() == 21);
        }

        {
          expected<int, int> e = 21;
          auto ret = e.transform(ret_void);
          REQUIRE(ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> const e = 21;
          auto ret = e.transform(ret_void);
          REQUIRE(ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> e = 21;
          auto ret = std::move(e).transform(ret_void);
          REQUIRE(ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> const e = 21;
          auto ret = std::move(e).transform(ret_void);
          REQUIRE(ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = e.transform(ret_void);
          REQUIRE(! ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = e.transform(ret_void);
          REQUIRE(! ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> e(unexpect, 21);
          auto ret = std::move(e).transform(ret_void);
          REQUIRE(! ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        {
          expected<int, int> const e(unexpect, 21);
          auto ret = std::move(e).transform(ret_void);
          REQUIRE(! ret);
          STATIC_REQUIRE((std::is_same<decltype(ret), expected<void, int>>::value));
        }

        // mapping functions which return references
        {
          expected<int, int> e(42);
          auto ret = e.transform([](int& i) -> int& { return i; });
          REQUIRE(ret);
          REQUIRE(ret == 42);
        }
      }
    }
  }  // Result

  SECTION("Optional", "[types.optional]") {
    SECTION("Make optional") {
      auto o1 = make_optional(42);
      auto o2 = optional<int>(42);

      constexpr bool is_same = std::is_same<decltype(o1), optional<int>>::value;
      REQUIRE(is_same);
      REQUIRE(o1 == o2);

      auto o3 = make_optional<std::tuple<int, int, int, int>>(0, 1, 2, 3);
      REQUIRE(std::get<0>(*o3) == 0);
      REQUIRE(std::get<1>(*o3) == 1);
      REQUIRE(std::get<2>(*o3) == 2);
      REQUIRE(std::get<3>(*o3) == 3);

      auto o4 = make_optional<std::vector<int>>({0, 1, 2, 3});
      REQUIRE(o4.value()[0] == 0);
      REQUIRE(o4.value()[1] == 1);
      REQUIRE(o4.value()[2] == 2);
      REQUIRE(o4.value()[3] == 3);

      auto o5 = make_optional<takes_init_and_variadic>({0, 1}, 2, 3);
      REQUIRE(o5->v[0] == 0);
      REQUIRE(o5->v[1] == 1);
      REQUIRE(std::get<0>(o5->t) == 2);
      REQUIRE(std::get<1>(o5->t) == 3);

      auto i = 42;
      auto o6 = make_optional<int&>(i);
      REQUIRE((std::is_same<decltype(o6), optional<int&>>::value));
      REQUIRE(o6);
      REQUIRE(*o6 == 42);
    }
  }  // Optional

  SECTION("Angle", "[types.angle]") {
    SECTION("Wrap") {
      auto constexpr PI = rolly::numbers::pi_v<f32>;
      auto constexpr FRAC_PI_2 = PI / 2.0F;

      REQUIRE(angle<f32>::from_radians(0.0).positive() == angle<f32>::zero());
      REQUIRE(angle<f32>::from_radians(FRAC_PI_2).positive() == angle<f32>::half_pi());
      REQUIRE(
        angle<f32>::from_radians(-FRAC_PI_2).positive()
        == angle<f32>::from_radians(3.0F * FRAC_PI_2)
      );
      REQUIRE(
        angle<f32>::from_radians(3.0F * FRAC_PI_2).positive()
        == angle<f32>::from_radians(3.0F * FRAC_PI_2)
      );
      REQUIRE(angle<f32>::from_radians(5.0F * FRAC_PI_2).positive() == angle<f32>::half_pi());
      REQUIRE(angle<f32>::from_radians(2.0 * PI).positive() == angle<f32>::zero());
      REQUIRE(angle<f32>::from_radians(-2.0 * PI).positive() == angle<f32>::zero());
      REQUIRE(angle<f32>::from_radians(PI).positive() == angle<f32>::pi());
      REQUIRE(angle<f32>::from_radians(-PI).positive() == angle<f32>::pi());
      REQUIRE(angle<f32>::from_radians(FRAC_PI_2).signed_() == angle<f32>::half_pi());
      REQUIRE(
        std::abs(*angle<f32>::from_radians(5.0F * FRAC_PI_2).signed_() - *angle<f32>::half_pi())
        < 0.0001F
      );
      REQUIRE(
        std::abs(*angle<f32>::from_radians(3.0F * FRAC_PI_2).signed_() + *angle<f32>::half_pi())
        < 0.0001F
      );
      REQUIRE(angle<f32>::from_radians(2.0 * PI).signed_() == angle<f32>::zero());
      REQUIRE(angle<f32>::from_radians(-2.0 * PI).signed_() == angle<f32>::zero());
      REQUIRE(angle<f32>::from_radians(PI).signed_() == angle<f32>::pi());
      REQUIRE(angle<f32>::from_radians(-PI).signed_() == angle<f32>::pi());
    }

    SECTION("Lerp") {
      auto const a = angle<f32>::from_radians(1.0F);
      auto const b = angle<f32>::from_radians(2.0F);

      REQUIRE_THAT(*a.lerp(b, 0.25F), WithinRel(1.25F));
      REQUIRE_THAT(*a.lerp(b, 0.5F), WithinRel(1.5F));
      REQUIRE_THAT(*a.lerp(b, 0.75F), WithinRel(1.75F));
      REQUIRE_THAT(*a.lerp(b + 2.0F * numbers::pi, 0.75F), WithinRel(1.75F));
      REQUIRE_THAT(*a.lerp(b - 2.0F * numbers::pi, 0.75F), WithinRel(1.75F));
      REQUIRE_THAT(*a.lerp(b + 10.0F * numbers::pi, 0.75F), WithinRel(1.75F));
    }

    SECTION("Sum") {
      auto const angles = std::vector<angle<f32>> {
        angle<f32>::from_radians(1.0F),
        angle<f32>::from_radians(2.0F),
        angle<f32>::from_radians(3.0F)
      };
      auto const sum = angle<f32>::from_radians(6.0F);

      REQUIRE(angles[0] + angles[1] + angles[2] == sum);
      REQUIRE(std::accumulate(angles.cbegin(), angles.cend(), angle<f32>::zero()) == sum);
    }

    SECTION("Format") {
      auto const a = angle<f32>::from_degrees(1.0F);
      std::stringstream ss;
      ss << a;
      REQUIRE(ss.str() == "1.00°");
      REQUIRE(fmt::format("{}", a) == "1.00°");
    }
  }  // Angle

  SECTION("Point 2D", "[types.point2d]") {
    SECTION("Negation") {
      REQUIRE(-point2d(1.0, 2.0) == point2d(-1.0, -2.0));
      REQUIRE(-point2d(0.0, 0.0) == point2d(-0.0, -0.0));
      REQUIRE(-point2d(-1.0, -2.0) == point2d(1.0, 2.0));
    }

    SECTION("AddSize") {
      constexpr auto p1 = point2d(1.0, 2.0);
      constexpr auto p2 = size2d(3.0, 4.0);

      REQUIRE(p1 + p2 == point2d(4.0, 6.0));
    }

    SECTION("AddAssignSize") {
      auto p1 = point2d(1.0, 2.0);
      constexpr auto p2 = size2d(3.0, 4.0);

      REQUIRE((p1 += p2) == point2d(4.0, 6.0));
    }

    SECTION("Subtract") {
      constexpr auto p1 = point2d(1.0, 2.0);
      constexpr auto p2 = point2d(3.0, 4.0);

      REQUIRE(p1 - p2 == point2d(-2.0, -2.0));
    }

    SECTION("SubtractAssign") {
      auto p1 = point2d(1.0, 2.0);
      constexpr auto p2 = point2d(3.0, 4.0);

      REQUIRE((p1 -= p2) == point2d(-2.0, -2.0));
    }

    SECTION("SubtractSize") {
      constexpr auto p1 = point2d(1.0, 2.0);
      constexpr auto p2 = size2d(3.0, 4.0);

      REQUIRE(p1 - p2 == point2d(-2.0, -2.0));
    }

    SECTION("SubtractAssignSize") {
      auto p1 = point2d(1.0, 2.0);
      constexpr auto p2 = size2d(3.0, 4.0);

      REQUIRE((p1 -= p2) == point2d(-2.0, -2.0));
    }

    SECTION("MulScalar") {
      constexpr auto p1 = point2d(3.0, 5.0);

      REQUIRE(p1 * 2.0 == point2d(6.0, 10.0));
      REQUIRE(p1 * 5.0 == point2d(15.0, 25.0));
      REQUIRE(p1 * -1.0 == point2d(-3.0, -5.0));
    }

    SECTION("MulAssignScalar") {
      auto p1 = point2d(3.0, 5.0);

      REQUIRE((p1 *= 2.0) == point2d(6.0, 10.0));
      REQUIRE((p1 *= 2.0) == point2d(12.0, 20.0));
      REQUIRE((p1 *= -0.5) == point2d(-6.0, -10.0));
    }

    SECTION("DivScalar") {
      constexpr auto p1 = point2d(3.0, 5.0);

      REQUIRE(p1 / 2.0 == point2d(1.5, 2.5));
      REQUIRE(p1 / 5.0 == point2d(0.6, 1.0));
      REQUIRE(p1 / -1.0 == point2d(-3.0, -5.0));
    }

    SECTION("DivAssignScalar") {
      auto p1 = point2d(3.0, 5.0);

      REQUIRE((p1 /= 2.0) == point2d(1.5, 2.5));
      REQUIRE((p1 /= 2.0) == point2d(0.75, 1.25));
    }

    SECTION("Format") {
      constexpr auto p1 = point2d(1.0, 2.0);
      auto const res1 = fmt::format("{}", p1);
      constexpr auto p2 = point2d(1, 2);
      auto const res2 = fmt::format("{}", p2);
      REQUIRE(res1 == "[1.00, 2.00]");
      REQUIRE(res2 == "[1, 2]");
    }

    SECTION("Equality") {
      REQUIRE(point2d(1.0, 2.0) == point2d(1.0, 2.0));
      REQUIRE_FALSE(point2d(1.0, 2.0) == point2d(1.0, 3.0));
      REQUIRE_FALSE(point2d(1.0, 2.0) == point2d(2.0, 2.0));
    }

    SECTION("RemEuclid") {
      constexpr auto p = point2d(7.0, -7.0);
      constexpr auto sp = point2d(4.0, -4.0);
      constexpr auto s = size2d(4.0, -4.0);

      REQUIRE(p.rem_euclid(sp) == point2d(3.0, 1.0));
      REQUIRE((-p).rem_euclid(sp) == point2d(1.0, 3.0));
      REQUIRE(p.rem_euclid(-sp) == point2d(3.0, 1.0));

      REQUIRE(p.rem_euclid(s) == point2d(3.0, 1.0));
      REQUIRE((-p).rem_euclid(s) == point2d(1.0, 3.0));
      REQUIRE(p.rem_euclid(-s) == point2d(3.0, 1.0));
    }

    SECTION("DivEuclid") {
      constexpr auto p = point2d(7.0, -7.0);
      constexpr auto sp = point2d(4.0, -4.0);
      constexpr auto s = sp.to_size2d();

      REQUIRE(p.div_euclid(sp) == point2d(1.0, 2.0));
      REQUIRE((-p).div_euclid(sp) == point2d(-2.0, -1.0));
      REQUIRE(p.div_euclid(-sp) == point2d(-1.0, -2.0));

      REQUIRE(p.div_euclid(s) == point2d(1.0, 2.0));
      REQUIRE((-p).div_euclid(s) == point2d(-2.0, -1.0));
      REQUIRE(p.div_euclid(-s) == point2d(-1.0, -2.0));
    }

#if defined(ROLLY_QT_CORE)
    SECTION("ToQPoint") {
      constexpr auto p = point2d(1.0, 2.0);

      REQUIRE(p.to_qpoint() == QPoint(1, 2));
    }

    SECTION("ToQPointF") {
      constexpr auto p = point2d(1.0, 2.0);

      REQUIRE(p.to_qpointf() == QPointF(1.0, 2.0));
    }

    SECTION("FromQPoint") {
      auto const p = QPoint(1, 2);

      REQUIRE(point2d(p) == point2d(1.0F, 2.0F));
    }
#endif

    SECTION("Map") {
      constexpr auto p = point2d(1.0, 2.0);
      constexpr auto expected = point2d<i32>(2, 4);
      auto fn = [](f64 x) -> i32 { return x * 2; };
      REQUIRE(p.map(fn) == expected);
    }

    SECTION("Zip") {
      constexpr auto p = point2d(1.0, 2.0);
      constexpr auto q = point2d(3.0, 4.0);
      constexpr auto got = p.zip(q, [](f64 x, f64 y) -> f64 { return x + y; });
      constexpr auto expected = point2d(4.0, 6.0);
      REQUIRE(got.to_size2d() == expected.to_size2d());
    }

    SECTION("AddVec") {
      constexpr auto p = point2d(1.0, 2.0);
      constexpr auto v = vector2d(3.0, 4.0);
      constexpr auto got = p + v;
      constexpr auto expected = point2d(4.0, 6.0);
      REQUIRE(got == expected);
    }

    SECTION("SubVec") {
      constexpr auto p = point2d(1.0, 2.0);
      constexpr auto v = vector2d(3.0, 4.0);
      constexpr auto got = p - v;
      constexpr auto expected = point2d(-2.0, -2.0);
      REQUIRE(got == expected);
    }
  }  // Point2D

  SECTION("Size 2D", "[types.size2d]") {
    SECTION("Area") {
      auto const p = size2d(1.5, 2.0);

      REQUIRE(p.x() == 1.5);
      REQUIRE(p.y() == 2.0);
      REQUIRE(p.area() == 3.0);
    }

    SECTION("FromTuple") {
      auto const p = size2d<f64>::from_tuple(std::tuple<double, double> {1.5, 2.0});

      REQUIRE(p.x() == 1.5);
      REQUIRE(p.y() == 2.0);
      REQUIRE(p.area() == 3.0);
    }

    SECTION("Negation") {
      REQUIRE(-size2d(1.0, 2.0) == size2d(-1.0, -2.0));
      REQUIRE(-size2d(0.0, 0.0) == size2d(-0.0, -0.0));
      REQUIRE(-size2d(-1.0, -2.0) == size2d(1.0, 2.0));
    }

    SECTION("Add") {
      REQUIRE(size2d(1.0, 2.0) + size2d(3.0, 4.0) == size2d(4.0, 6.0));
      REQUIRE(size2d(1.0, 2.0) + size2d(0.0, 0.0) == size2d(1.0, 2.0));
      REQUIRE(size2d(1.0, 2.0) + size2d(-3.0, -4.0) == size2d(-2.0, -2.0));
      REQUIRE(size2d(0.0, 0.0) + size2d(0.0, 0.0) == size2d(0.0, 0.0));
    }

    SECTION("AddAssign") {
      REQUIRE((size2d(1.0, 2.0) += size2d(3.0, 4.0)) == size2d(4.0, 6.0));
      REQUIRE((size2d(1.0, 2.0) += size2d(0.0, 0.0)) == size2d(1.0, 2.0));
      REQUIRE((size2d(1.0, 2.0) += size2d(-3.0, -4.0)) == size2d(-2.0, -2.0));
      REQUIRE((size2d(0.0, 0.0) += size2d(0.0, 0.0)) == size2d(0.0, 0.0));
    }

    SECTION("Sum") {
      auto vec =
        std::vector<size2d<f32>> {size2d(1.0F, 2.0F), size2d(3.0F, 4.0F), size2d(5.0F, 6.0F)};
      auto acc = std::accumulate(vec.begin(), vec.end(), size2d());

      REQUIRE(acc == size2d(9.0F, 12.0F));
    }

    SECTION("Subtract") {
      REQUIRE(size2d(1.0, 2.0) - size2d(3.0, 4.0) == size2d(-2.0, -2.0));
      REQUIRE(size2d(1.0, 2.0) - size2d(0.0, 0.0) == size2d(1.0, 2.0));
      REQUIRE(size2d(1.0, 2.0) - size2d(-3.0, -4.0) == size2d(4.0, 6.0));
      REQUIRE(size2d(0.0, 0.0) - size2d(0.0, 0.0) == size2d(0.0, 0.0));
    }

    SECTION("SubtractAssign") {
      REQUIRE((size2d(1.0, 2.0) -= size2d(3.0, 4.0)) == size2d(-2.0, -2.0));
      REQUIRE((size2d(1.0, 2.0) -= size2d(0.0, 0.0)) == size2d(1.0, 2.0));
      REQUIRE((size2d(1.0, 2.0) -= size2d(-3.0, -4.0)) == size2d(4.0, 6.0));
      REQUIRE((size2d(0.0, 0.0) -= size2d(0.0, 0.0)) == size2d(0.0, 0.0));
    }

    SECTION("MultiplyByScalar") {
      REQUIRE(size2d(1.0, 2.0) * 3.0 == size2d(3.0, 6.0));
      REQUIRE(size2d(1.0, 2.0) * 0.0 == size2d(0.0, 0.0));
      REQUIRE(size2d(1.0, 2.0) * -3.0 == size2d(-3.0, -6.0));
      REQUIRE(size2d(0.0, 0.0) * 0.0 == size2d(0.0, 0.0));
    }

    SECTION("MultiplyAssignByScalar") {
      REQUIRE((size2d(1.0, 2.0) *= 3.0) == size2d(3.0, 6.0));
      REQUIRE((size2d(1.0, 2.0) *= 0.0) == size2d(0.0, 0.0));
      REQUIRE((size2d(1.0, 2.0) *= -3.0) == size2d(-3.0, -6.0));
      REQUIRE((size2d(0.0, 0.0) *= 0.0) == size2d(0.0, 0.0));
    }

    SECTION("DivideByScalar") {
      REQUIRE(size2d(1.0, 2.0) / 2.0 == size2d(0.5, 1.0));
      REQUIRE(size2d(1.0, 2.0) / -2.0 == size2d(-0.5, -1.0));
    }

    SECTION("DivideAssignByScalar") {
      REQUIRE((size2d(1.0, 2.0) /= 2.0) == size2d(0.5, 1.0));
      REQUIRE((size2d(1.0, 2.0) /= -2.0) == size2d(-0.5, -1.0));
    }

    SECTION("NanEmpty") {
      constexpr auto a = size2d(std::numeric_limits<f32>::quiet_NaN(), 2.0F);
      constexpr auto b = size2d(0.0F, std::numeric_limits<f32>::quiet_NaN());
      constexpr auto c = size2d(std::numeric_limits<f32>::quiet_NaN(), -2.0F);
      REQUIRE_FALSE(a);
      REQUIRE_FALSE(b);
      REQUIRE_FALSE(c);
    }

#if defined(ROLLY_QT_CORE)
    SECTION("ToQSize") {
      REQUIRE(size2d(1.0, 2.0).to_qsize() == QSize(1, 2));
      REQUIRE(size2d(0.0, 0.0).to_qsize() == QSize(0, 0));
      REQUIRE(size2d(-1.0, -2.0).to_qsize() == QSize(-1, -2));
    }

    SECTION("ToQSizeF") {
      REQUIRE(size2d(1.0, 2.0).to_qsizef() == QSizeF(1.0, 2.0));
      REQUIRE(size2d(0.0, 0.0).to_qsizef() == QSizeF(0.0, 0.0));
      REQUIRE(size2d(-1.0, -2.0).to_qsizef() == QSizeF(-1.0, -2.0));
    }
#endif  // defined(FL_QT_CORE)

    SECTION("ToPoint2D") {
      REQUIRE(size2d(1.0, 2.0).to_point2d() == point2d(1.0, 2.0));
      REQUIRE(size2d(0.0, 0.0).to_point2d() == point2d(0.0, 0.0));
      REQUIRE(size2d(-1.0, -2.0).to_point2d() == point2d(-1.0, -2.0));
    }
  }  // Size2D

  SECTION("Vector 2D", "[types.vector2d]") {
    SECTION("ScalarMul") {
      constexpr auto p1 = vector2d(3.0, 5.0);
      constexpr auto result = p1 * 5.0;

      REQUIRE(result.x() == 15.0);
      REQUIRE(result.y() == 25.0);
      REQUIRE(result == vector2d(15.0, 25.0));
    }

    SECTION("Dot") {
      constexpr auto p1 = vector2d(2.0, 7.0);
      constexpr auto p2 = vector2d(13.0, 11.0);
      REQUIRE(p1.dot(p2) == 103.0);
    }

    SECTION("Cross") {
      constexpr auto p1 = vector2d(4.0, 7.0);
      constexpr auto p2 = vector2d(13.0, 8.0);
      REQUIRE(p1.cross(p2) == -59.0);
    }

    SECTION("Normalize") {
      constexpr auto p1 = vector2d(4.0, 0.0);
      constexpr auto p2 = vector2d(3.0, -4.0);
      REQUIRE(p1.normalized().x() == 1.0);
      REQUIRE(p1.normalized().y() == 0.0);
      REQUIRE(p2.normalized().x() == 0.6);
      REQUIRE(p2.normalized().y() == -0.8);
    }

    SECTION("Length") {
      constexpr auto p1 = vector2d(3.0, 4.0);

      REQUIRE(p1.length_scalar() == 5.0);
    }

    SECTION("LengthSquared") {
      constexpr auto p1 = vector2d(3.0, 4.0);

      REQUIRE(p1.length_squared() == 25.0);
    }

    SECTION("Min") {
      constexpr auto p1 = vector2d(1.0, 3.0);
      constexpr auto p2 = vector2d(2.0, 2.0);

      REQUIRE(p1.min(p2) == vector2d(1.0, 2.0));
    }

    SECTION("Max") {
      constexpr auto p1 = vector2d(1.0, 3.0);
      constexpr auto p2 = vector2d(2.0, 2.0);

      REQUIRE(p1.max(p2) == vector2d(2.0, 3.0));
    }

    SECTION("AngleFromXAxis") {
      constexpr auto right = vector2d(10.0, 0.0);
      constexpr auto down = vector2d(0.0, 4.0);
      constexpr auto up = vector2d(0.0, -1.0);

      REQUIRE(right.angle_to_x_axis() == angle<f64>::zero());
      REQUIRE_THAT(down.angle_to_x_axis().value(), WithinAbs(angle<f64>::half_pi().value(), 0.01));
      REQUIRE_THAT(up.angle_to_x_axis().value(), WithinAbs(-angle<f64>::half_pi().value(), 0.01));
    }

    SECTION("AngleTo") {
      constexpr auto right = vector2d(10.0, 0.0);
      constexpr auto right2 = vector2d(1.0, 0.0);
      constexpr auto up = vector2d(0.0, -1.0);
      constexpr auto up_left = vector2d(-1.0, -1.0);

      REQUIRE(right.angle_to(right2) == angle<f64>::zero());
      REQUIRE_THAT(right.angle_to(up).value(), WithinAbs(-angle<f64>::half_pi().value(), 0.01));
      REQUIRE_THAT(up.angle_to(right).value(), WithinAbs(angle<f64>::half_pi().value(), 0.01));
      REQUIRE_THAT(up_left.angle_to(up).value(), WithinAbs(angle<f64>::quarter_pi().value(), 0.01));
    }

    SECTION("ProjectOnto") {
      constexpr auto v1 = vector2d(1.0, 2.0);
      constexpr auto x = vector2d(1.0, 0.0);
      constexpr auto y = vector2d(0.0, 1.0);

      REQUIRE(v1.project(x) == vector2d(1.0, 0.0));
      REQUIRE(v1.project(y) == vector2d(0.0, 2.0));
      REQUIRE(v1.project(-x) == vector2d(1.0, 0.0));
      REQUIRE(v1.project(x * 10.0) == vector2d(1.0, 0.0));
      REQUIRE(v1.project(v1 * 2.0) == v1);
      REQUIRE(v1.project(-v1) == v1);
    }

    SECTION("Add") {
      constexpr auto p1 = vector2d(1.0, 2.0);
      constexpr auto p2 = vector2d(3.0, 4.0);

      REQUIRE(p1 + p2 == vector2d(4.0, 6.0));
      REQUIRE(p1 + p2 == vector2d(4.0, 6.0));
    }

    SECTION("Sum") {
      auto vec = std::vector<vector2d<f32>> {
        vector2d(1.0F, 2.0F),
        vector2d(3.0F, 4.0F),
        vector2d(5.0F, 6.0F)
      };
      auto acc = std::accumulate(vec.begin(), vec.end(), vector2d());

      REQUIRE(acc == vector2d(9.0F, 12.0F));
    }

    SECTION("Sub") {
      constexpr auto p1 = vector2d(1.0, 2.0);
      constexpr auto p2 = vector2d(3.0, 4.0);

      REQUIRE(p1 - p2 == vector2d(-2.0, -2.0));
    }

    SECTION("SubAssign") {
      auto p1 = vector2d(1.0, 2.0);
      p1 -= vector2d(3.0, 4.0);

      REQUIRE(p1 == vector2d(-2.0, -2.0));
    }

    SECTION("MulAssign") {
      auto p1 = vector2d(1.0, 2.0);
      p1 *= 3.0;

      REQUIRE(p1 == vector2d(3.0, 6.0));
    }

    SECTION("DivAssign") {
      auto p1 = vector2d(3.0, 6.0);
      p1 /= 3.0;

      REQUIRE(p1 == vector2d(1.0, 2.0));
    }

    SECTION("Neg") {
      auto p1 = vector2d(1.0, 2.0);
      auto p2 = -p1;

      REQUIRE(p2 == vector2d(-1.0, -2.0));
    }

    SECTION("Swizzle") {
      auto p1 = vector2d(1.0, 2.0);

      REQUIRE(p1.inverted() == vector2d(2.0, 1.0));
    }

    SECTION("Reflect") {
      constexpr auto a = vector2d(1.0, 3.0);
      constexpr auto n1 = vector2d(0.0, -1.0);
      auto const n2 = vector2d(1.0, -1.0).normalized();

      REQUIRE(a.reflected(n1) == vector2d(1.0, -3.0));
      REQUIRE(a.reflected(n2) == vector2d(3.0, 1.0));
    }

    SECTION("Map") {
      constexpr auto p = vector2d(1.0, 2.0);
      constexpr auto expected = vector2d<i32>(2, 4);
      auto fn = [](f64 x) -> i32 { return x * 2; };
      REQUIRE(p.map(fn) == expected);
    }

    SECTION("Zip") {
      constexpr auto p = vector2d(1.0, 2.0);
      constexpr auto q = vector2d(3.0, 4.0);
      constexpr auto got = p.zip(q, [](f64 x, f64 y) -> f64 { return x + y; });
      constexpr auto expected = vector2d(4.0, 6.0);
      REQUIRE(got.to_point2d() == expected.to_point2d());
    }
  }  // Vector2D
}
