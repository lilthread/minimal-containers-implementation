#include <gtest/gtest.h>
#include "Any.hpp"
#include <any>

TEST(Any, CtorDefault) {
  not_std::Any<int> x(1);

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
