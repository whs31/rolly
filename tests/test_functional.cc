#include <rolly/functional.h>
#include <catch2/catch_all.hpp>

using namespace rolly;

static int sum = 0;

struct s {
  static void s1(int i) { sum += i; }

  static void s2(int i) noexcept { sum += 2 * i; }

  void f1(int i) { sum += i; }

  void f2(int i) const { sum += i; }

  void f3(int i) volatile { sum += i; }

  void f4(int i) const volatile { sum += i; }

  void f5(int i) noexcept { sum += i; }

  void f6(int i) const noexcept { sum += i; }

  void f7(int i) volatile noexcept { sum += i; }

  void f8(int i) const volatile noexcept { sum += i; }
};

struct Sender {
  void trigger() { this->triggered(); }

  rolly::signal<> triggered;
};

TEST_CASE("Functional", "[functional]") {
  SECTION("Signals") {
    SECTION("Lambda connection") {
      auto sig = rolly::signal<int>();
      auto p = s();
      sig.connect(&s::f1, &p);
      REQUIRE(sig.slot_count() == 1);
      sig.connect(&s::f2, &p);
      REQUIRE(sig.slot_count() == 2);
      sig.connect(&s::f3, &p);
      REQUIRE(sig.slot_count() == 3);
      sig.connect(&s::f4, &p);
      REQUIRE(sig.slot_count() == 4);
      sig.connect(&s::f5, &p);
      REQUIRE(sig.slot_count() == 5);
      sig.connect(&s::f6, &p);
      REQUIRE(sig.slot_count() == 6);
      sig.connect(&s::f7, &p);
      REQUIRE(sig.slot_count() == 7);
      sig.connect(&s::f8, &p);
      REQUIRE(sig.slot_count() == 8);
      sig.disconnect(&s::f1, &p);
      REQUIRE(sig.slot_count() == 7);
      sig.disconnect(&s::f2, &p);
      REQUIRE(sig.slot_count() == 6);
      sig.disconnect(&s::f3, &p);
      REQUIRE(sig.slot_count() == 5);
      sig.disconnect(&s::f4, &p);
      REQUIRE(sig.slot_count() == 4);
      sig.disconnect(&s::f5, &p);
      REQUIRE(sig.slot_count() == 3);
      sig.disconnect(&s::f6, &p);
      REQUIRE(sig.slot_count() == 2);
      sig.disconnect(&s::f7, &p);
      REQUIRE(sig.slot_count() == 1);
      sig.disconnect(&s::f8, &p);
      REQUIRE(sig.slot_count() == 0);
    }

    SECTION("Example sender/receiver") {
      Sender sender;
      bool triggered = false;
      sender.triggered.connect([&]() { triggered = true; });
      REQUIRE(not triggered);
      sender.trigger();
      REQUIRE(triggered);
    }
  }
}
