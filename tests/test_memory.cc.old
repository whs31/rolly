#include <cstdint>
#include <gtest/gtest.h>
#include <floppy/floppy.h>

// NOLINTBEGIN
struct A
{
  A() = default;
  A(int a_, int b_) : a(a_), b(b_) {}
  int a = 5;
  int b = 42;
  [[nodiscard]] virtual auto name() const -> std::string { return "A"; }
};

struct B : public A
{
  B() = default;
  B(int a_, int b_, int c_, int d_) : A(a_, b_), c(c_), d(d_) {}
  int c = 3;
  int d = 7;
  [[nodiscard]] virtual auto name() const -> std::string override { return "B"; }
};
// NOLINTEND

TEST(Memory, ConstructorVariadic)
{
  auto b = floppy::box<B>::make(1, 2, 3, 7);
  EXPECT_EQ(b->name(), "B");
  EXPECT_EQ(b->a, 1);
  EXPECT_EQ(b->b, 2);
  EXPECT_EQ(b->c, 3);
  EXPECT_EQ(b->d, 7);
  EXPECT_EQ(b.ref().name(), "B");
}

TEST(Memory, ConstructorFromUnique)
{
  auto b_unique = std::make_unique<B>(1, 2, 3, 7);
  auto b = floppy::box<B>::make(std::move(b_unique));
  EXPECT_EQ(b->name(), "B");
  EXPECT_EQ(b->a, 1);
  EXPECT_EQ(b->b, 2);
  EXPECT_EQ(b->c, 3);
  EXPECT_EQ(b->d, 7);
  EXPECT_EQ(b.ref().name(), "B");
}

TEST(Memory, MakeBox)
{
  auto b = floppy::make_box<B>(0, 0, 0, 0);
  EXPECT_EQ(b->name(), "B");
  EXPECT_EQ(b->a, 0);
  EXPECT_EQ(b->b, 0);
  EXPECT_EQ(b->c, 0);
  EXPECT_EQ(b->d, 0);
  EXPECT_EQ(b.ref().name(), "B");
}

TEST(Memory, DefaultBox)
{
  auto const b = floppy::box<B>();
  EXPECT_EQ(b->name(), "B");
  EXPECT_EQ(b->a, 5);
  EXPECT_EQ(b->b, 42);
  EXPECT_EQ(b->c, 3);
  EXPECT_EQ(b->d, 7);
  EXPECT_EQ(b.ref().name(), "B");
}

TEST(Memory, Getters)
{
  auto b = floppy::box<B>();
  EXPECT_EQ(b.ptr_mut()->name(), "B");
  EXPECT_EQ(b.ptr()->name(), "B");
  EXPECT_EQ(b.ref_mut().name(), "B");
  EXPECT_EQ(b.ref().name(), "B");
  EXPECT_EQ((*b).name(), "B");
  EXPECT_EQ(b->name(), "B");
}

TEST(Memory, BoxAsUnique)
{
  auto b = floppy::box<B>();
  auto& b_unique = b.as_unique_ptr();
  EXPECT_EQ(b_unique->name(), "B");
}

TEST(Memory, BoxLeak)
{
  auto b = floppy::box<B>();
  auto* pointer = b.leak();
  EXPECT_THROW(std::ignore = b.ref(), floppy::contracts::contract_violation);
  EXPECT_EQ(pointer->name(), "B");
  delete pointer; // NOLINT(*-owning-memory)
}

TEST(Memory, BoxAs)
{
  auto b = floppy::box<B>();
  auto* a = b.as<A>();
  EXPECT_EQ(a->name(), "B");
  EXPECT_EQ(a->a, 5);
  EXPECT_EQ(a->b, 42);
}

TEST(Memory, Box)
{
  auto b = floppy::box<B>();
  auto& a = b.downcast<A>().value().get();
  EXPECT_EQ(a.name(), "B");
  EXPECT_EQ(a.a, 5);
  EXPECT_EQ(a.b, 42);
}