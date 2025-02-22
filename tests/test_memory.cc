#include <rll/memory.h>
#include <catch2/catch_all.hpp>

using namespace rll;

struct A {
  virtual ~A() {}

  virtual std::string_view name() const { return "A"; }
};

struct B : public A {
  virtual ~B() {}

  virtual std::string_view name() const override { return "B"; }
};

TEST_CASE("Memory", "[memory]") {
  SECTION("ObserverPtr") {
    SECTION("Constructor") {
      observer_ptr<int> ptr;
      REQUIRE(ptr.get() == nullptr);

      int value = 42;
      observer_ptr<int> ptr2(&value);
      REQUIRE(ptr2.get() == &value);

      observer_ptr<int> ptr3 = ptr2;
      REQUIRE(ptr3.get() == &value);

      observer_ptr<int> ptr4(std::move(ptr2));
      REQUIRE(ptr4.get() == &value);
      REQUIRE(ptr2.get() != nullptr);
      REQUIRE(ptr4.ref() == 42);
      REQUIRE(*ptr4 == 42);
    }

    SECTION("Dereference") {
      int value = 42;
      observer_ptr<int> ptr(&value);
      REQUIRE(ptr.ref() == 42);
      REQUIRE(*ptr == 42);
    }

    SECTION("Swap") {
      int value1 = 42;
      int value2 = 43;
      observer_ptr<int> ptr1(&value1);
      observer_ptr<int> ptr2(&value2);
      ptr1.swap(ptr2);
      REQUIRE(ptr1.get() == &value2);
      REQUIRE(ptr2.get() == &value1);
    }

    SECTION("Reset") {
      int value = 42;
      observer_ptr<int> ptr(&value);
      ptr.reset();
      REQUIRE(ptr.get() == nullptr);
    }

    SECTION("Release") {
      int value = 42;
      observer_ptr<int> ptr(&value);
      REQUIRE(ptr.release() == &value);
      REQUIRE(ptr.get() == nullptr);
    }

    SECTION("Hash") {
      int value = 42;
      observer_ptr<int> ptr(&value);
      REQUIRE(std::hash<observer_ptr<int>> {}(ptr) == std::hash<int*> {}(&value));
    }

    SECTION("Equal") {
      int value = 42;
      observer_ptr<int> ptr1(&value);
      observer_ptr<int> ptr2(&value);
      REQUIRE(ptr1 == ptr2);
    }

    SECTION("NotEqual") {
      int value1 = 42;
      int value2 = 43;
      observer_ptr<int> ptr1(&value1);
      observer_ptr<int> ptr2(&value2);
      REQUIRE(ptr1 != ptr2);
    }

    SECTION("FromUniquePtr") {
      auto const ptr = std::make_unique<int>(42);
      REQUIRE(*ptr == 42);

      auto obs = observer_ptr<int>(ptr);
      REQUIRE(obs.ref() == 42);
    }
  }
}
