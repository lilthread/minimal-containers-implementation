#include <gtest/gtest.h>
#include "Any.hpp"
#include <string>

TEST(Any, CtorDefault) {
  not_std::Any x(1);

  EXPECT_EQ(x.type(), typeid(int));
  EXPECT_TRUE(x.has_value());
  EXPECT_EQ(not_std::any_cast<int>(x), 1);

  x.reset();
  EXPECT_TRUE(not x.has_value());
  EXPECT_EQ(x.type(), typeid(void));
}

TEST(Any, CtorAndNewValue) {
  not_std::Any x = "Hello, World!"; 
  EXPECT_EQ(x.type(), typeid(char const*));
  EXPECT_EQ(not_std::any_cast<char const*>(x), "Hello, World!");
  x = 10;
  EXPECT_EQ(x.type(), typeid(int));
}

struct Point {
  int x, y;
  bool operator==(const Point&) const = default;
};

TEST(Any, CastStruct) { 
  not_std::Any a(Point{1, 2});
  EXPECT_EQ(Point(1, 2), not_std::any_cast<Point>(a));
}

TEST(Any, ReplaceSameType) { 
  not_std::Any a(Point{1, 2});
  a = Point({5, 5});
  EXPECT_EQ(Point(5, 5), not_std::any_cast<Point>(a));
}

TEST(Any, CopyConstructor) {
  not_std::Any a = "copy";
  not_std::Any b(a);

  EXPECT_EQ(b.type(), typeid(char const*));
  EXPECT_EQ(not_std::any_cast<char const*>(b), "copy");
}


TEST(Any, CopyAssignment) {
  not_std::Any a = 123;
  not_std::Any b;

  b = a;

  EXPECT_EQ(b.type(), typeid(int));
  EXPECT_EQ(not_std::any_cast<int>(b), 123);
}

TEST(Any, SelfAssignment) {
  not_std::Any a = 99;

  a = a;

  EXPECT_EQ(a.type(), typeid(int));
  EXPECT_EQ(not_std::any_cast<int>(a), 99);
}

struct Large {
  int data[256];

  bool operator==(const Large& other) const {
    for (int i = 0; i < 256; ++i)
      if (data[i] != other.data[i])
        return false;
    return true;
  }
};

TEST(Any, LargeObject) {
  Large l{};
  for (int i = 0; i < 256; ++i)
    l.data[i] = i;

  not_std::Any a = l;

  EXPECT_EQ(not_std::any_cast<Large>(a), l);
}

TEST(Any, StoreZero) {
  not_std::Any a = 0;

  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(not_std::any_cast<int>(a), 0);
}


TEST(Any, StoreNullptr) {
  not_std::Any a = nullptr;

  EXPECT_EQ(a.type(), typeid(std::nullptr_t));
  EXPECT_EQ(not_std::any_cast<std::nullptr_t>(a), nullptr);
}

TEST(Any, MultipleReassignments) {
  not_std::Any a = 1;

  a = std::string("abc");
  EXPECT_EQ(not_std::any_cast<std::string>(a), "abc");

  a = 2.5;
  EXPECT_DOUBLE_EQ(not_std::any_cast<double>(a), 2.5);

  a = false;
  EXPECT_FALSE(not_std::any_cast<bool>(a));
}
