#include <rolly/types.h>

#include <sstream>
#include <type_traits>
#include <vector>
#include <string>
#include <numeric>
#include <catch2/catch_all.hpp>

using namespace rolly;
using Catch::Matchers::WithinRel;

struct takes_init_and_variadic {
  std::vector<int> v;
  std::tuple<int, int> t;

  template <class... Args>
  takes_init_and_variadic(std::initializer_list<int> l, Args&&... args)
    : v(l)
    , t(std::forward<Args>(args)...) {}
};

TEST_CASE("Types", "[types]") {
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

    SECTION("ParseFail") {
      auto const invalid = guid("7bcd757f-5b10-4f9b-af69-1a1f226f3baskdfmsadf3e");

      REQUIRE_FALSE(invalid.valid());
      REQUIRE_FALSE(invalid);
    }

    SECTION("Literal") { REQUIRE(s1 == "7bcd757f-5b10-4f9b-af69-1a1f226f3b3e"_guid); }
  }  // GUID

#ifdef ___rolly_cxx20___
  SECTION("Result", "[types.result]") {
    SECTION("Assertions") {
      expected<int, int> o1 = 42;
      REQUIRE_THROWS(o1.error());

      expected<int, int> o2 {unexpect, 0};
      REQUIRE_THROWS(*o2);

      struct foo {
        int bar;
      };
      expected<struct foo, int> o3 {unexpect, 0};
      REQUIRE_THROWS(o3->bar);
    }

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
#endif

  SECTION("Angle", "[types.angle]") {
    SECTION("Wrap") {
      auto constexpr PI = rolly::numbers::pi_v<f32>;
      auto constexpr FRAC_PI_2 = PI / 2.0F;

      REQUIRE(angle<f32>::from_radians(0.0).positive() == angle<f32>::zero());
      REQUIRE(angle<f32>::from_radians(FRAC_PI_2).positive() == angle<f32>::half_pi());
      REQUIRE(angle<f32>::from_radians(-FRAC_PI_2).positive() == angle<f32>::from_radians(3.0F * FRAC_PI_2));
      REQUIRE(angle<f32>::from_radians(3.0F * FRAC_PI_2).positive() == angle<f32>::from_radians(3.0F * FRAC_PI_2));
      REQUIRE(angle<f32>::from_radians(5.0F * FRAC_PI_2).positive() == angle<f32>::half_pi());
      REQUIRE(angle<f32>::from_radians(2.0 * PI).positive() == angle<f32>::zero());
      REQUIRE(angle<f32>::from_radians(-2.0 * PI).positive() == angle<f32>::zero());
      REQUIRE(angle<f32>::from_radians(PI).positive() == angle<f32>::pi());
      REQUIRE(angle<f32>::from_radians(-PI).positive() == angle<f32>::pi());
      REQUIRE(angle<f32>::from_radians(FRAC_PI_2).signed_() == angle<f32>::half_pi());
      REQUIRE(std::abs(*angle<f32>::from_radians(5.0F * FRAC_PI_2).signed_() - *angle<f32>::half_pi()) < 0.0001F);
      REQUIRE(std::abs(*angle<f32>::from_radians(3.0F * FRAC_PI_2).signed_() + *angle<f32>::half_pi()) < 0.0001F);
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
  }
}
