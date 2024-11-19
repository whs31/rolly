#include <rolly/types.h>

#include <sstream>
#include <type_traits>
#include <vector>
#include <string>
#include <numeric>
#include <catch2/catch_all.hpp>

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
  }

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
  }
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
  }
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
  }
}
