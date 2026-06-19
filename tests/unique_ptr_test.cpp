#include <gtest/gtest.h>
#include "UniquePtr.hpp"

namespace {
struct something final {
  something(int v = 0) : number(v){}
  int getValue() { return number; }
  int number{};
};
}

TEST(UniquePtr, DefaultConstructor) {
  not_std::UniquePtr<int> ptr;
  EXPECT_EQ(ptr.get(), nullptr);
}

TEST(UniquePtr, RawPointerConstructor) {
  not_std::UniquePtr<int> ptr(new int(5));
  EXPECT_EQ(*ptr, 5);
}

TEST(UniquePtr, DereferenceOperator) {
  not_std::UniquePtr<int> ptr(new int(10));
  EXPECT_EQ(*ptr, 10);
}

TEST(UniquePtr, ArrowOperator) {
  not_std::UniquePtr<something> ptr(new something(42));
  EXPECT_EQ(ptr->getValue(), 42);
}

TEST(UniquePtr, GetMethod) {
  int* raw = new int(7);
  not_std::UniquePtr<int> ptr(raw);
  EXPECT_EQ(ptr.get(), raw);
}

TEST(UniquePtr, BoolTrue) {
  not_std::UniquePtr<int> ptr(new int(1));
  EXPECT_TRUE(static_cast<bool>(ptr));
}

TEST(UniquePtr, BoolFalse) {
  not_std::UniquePtr<int> ptr;
  EXPECT_FALSE(static_cast<bool>(ptr));
}

TEST(UniquePtr, ResetWithValue) {
  not_std::UniquePtr<int> ptr(new int(1));
  ptr.reset(new int(99));
  EXPECT_EQ(*ptr, 99);
}

TEST(UniquePtr, ResetNull) {
  not_std::UniquePtr<int> ptr(new int(1));
  ptr.reset();
  EXPECT_EQ(ptr.get(), nullptr);
}

TEST(UniquePtr, Release) {
  int* raw = new int(123);
  not_std::UniquePtr<int> ptr(raw);

  int* out = ptr.release();

  EXPECT_EQ(out, raw);
  EXPECT_EQ(ptr.get(), nullptr);

  delete out;
}

TEST(UniquePtr, MoveConstructor) {
  not_std::UniquePtr<int> a(new int(55));
  not_std::UniquePtr<int> b(std::move(a));

  EXPECT_EQ(*b, 55);
  EXPECT_EQ(a.get(), nullptr);
}

TEST(UniquePtr, MoveAssignment) {
  not_std::UniquePtr<int> a(new int(10));
  not_std::UniquePtr<int> b;

  b = std::move(a);

  EXPECT_EQ(*b, 10);
  EXPECT_EQ(a.get(), nullptr);
}


TEST(UniquePtr, SelfMove) {
  not_std::UniquePtr<int> ptr(new int(10));
  ptr = std::move(ptr);

  EXPECT_EQ(*ptr, 10);
}

TEST(UniquePtr, Swap) {
  not_std::UniquePtr<int> a(new int(1));
  not_std::UniquePtr<int> b(new int(2));

  a.swap(b);

  EXPECT_EQ(*a, 2);
  EXPECT_EQ(*b, 1);
}

TEST(UniquePtr, MakeUniqueBasic) {
    auto ptr = not_std::MakeUnique<int>(42);
    EXPECT_EQ(*ptr, 42);
}

TEST(UniquePtr, MakeUniqueStruct) {
  auto ptr = not_std::MakeUnique<something>(99);
  EXPECT_EQ(ptr->getValue(), 99);
}

TEST(UniquePtr, MakeUniqueArray) {
  not_std::UniquePtr<int[]> ptr = not_std::MakeUniqueArray<int>(5);
  for (std::size_t i = 0; i < 5; i++) {
    ptr[i] = i;
  }
  EXPECT_EQ(ptr[0], 0);
  EXPECT_EQ(ptr[1], 1);
  EXPECT_EQ(ptr[2], 2);
  EXPECT_EQ(ptr[3], 3);
  EXPECT_EQ(ptr[4], 4);
}
