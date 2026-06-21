#include <cstdint>
#include <gtest/gtest.h>
#include <type_traits>
#include "Vector.hpp"

namespace {
struct Trivial final {
  std::int32_t a;
  double b;
};
static_assert(std::is_trivially_copyable_v<Trivial>);
} // namespace not_std


TEST(VectorTest, DefaultConstructor) {
  not_std::Vector<Trivial> v;
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, PushBackSingleElement) {
  not_std::Vector<Trivial> v;

  v.push_back({1, 2.0f});

  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0].a, 1);
  EXPECT_FLOAT_EQ(v[0].b, 2.0f);
}

TEST(VectorTest, PushBackMultipleElements) {
  not_std::Vector<Trivial> v;

  for (int i = 0; i < 10; i++)
    v.push_back({i, float(i)});

  EXPECT_EQ(v.size(), 10);

  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(v[i].a, i);
    EXPECT_FLOAT_EQ(v[i].b, float(i));
  }
}

TEST(VectorTest, FrontBack) {
  not_std::Vector<Trivial> v{};
  v.push_back({1, 1.0f});
  v.push_back({2, 2.0f});
  v.push_back({3, 3.0f});

  ASSERT_EQ(v.front().a, 1);
  EXPECT_EQ(v.back().a, 3);
}

TEST(VectorTest, OperatorAccess) {
  not_std::Vector<Trivial> v;
  v.push_back({10, 10.5f});

  v[0].a = 99;

  EXPECT_EQ(v[0].a, 99);
}

TEST(VectorTest, AtValidAccess) {
  not_std::Vector<Trivial> v;
  v.push_back({5, 5.0f});

  EXPECT_NO_THROW(v.at(0));
  EXPECT_EQ(v.at(0).a, 5);
}

TEST(VectorTest, AtOutOfRange) {
  not_std::Vector<Trivial> v;
  v.push_back({1, 1.0f});

  EXPECT_THROW(v.at(1), std::out_of_range);
}

TEST(VectorTest, EraseMiddle) {
  not_std::Vector<Trivial> v;

  v.push_back({1, 1.0f});
  v.push_back({2, 2.0f});
  v.push_back({3, 3.0f});

  v.erase(v.begin() + 1);

  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0].a, 1);
  EXPECT_EQ(v[1].a, 3);
}

TEST(VectorTest, Clear) {
  not_std::Vector<Trivial> v;

  v.push_back({1, 1.0f});
  v.push_back({2, 2.0f});

  v.clear();

  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, Iteration) {
  not_std::Vector<Trivial> v;

  for (int i = 0; i < 5; i++)
    v.push_back({i, float(i)});

  int i = 0;
  for (auto& x : v) {
    EXPECT_EQ(x.a, i);
    i++;
  }

  EXPECT_EQ(i, 5);
}

TEST(VectorTest, ReverseIteration) {
  not_std::Vector<Trivial> v;

  for (int i = 0; i < 5; i++)
    v.push_back({i, float(i)});

  int expected = 4;
  for (auto it = v.rbegin(); it != v.rend(); it++) {
    EXPECT_EQ(it->a, expected);
    expected--;
  }

  EXPECT_EQ(expected, -1);
}
TEST(VectorTest, CopyConstructor) {
  not_std::Vector<Trivial> v1;

  v1.push_back({1, 1.0f});
  v1.push_back({2, 2.0f});

  not_std::Vector<Trivial> v2 = v1;

  EXPECT_EQ(v2.size(), 2);
  EXPECT_EQ(v2[0].a, 1);
  EXPECT_EQ(v2[1].a, 2);
}

TEST(VectorTest, MoveConstructor) {
  not_std::Vector<Trivial> v1;

  v1.push_back({1, 1.0f});
  v1.push_back({2, 2.0f});

  not_std::Vector<Trivial> v2 = std::move(v1);

  EXPECT_EQ(v2.size(), 2);
}

TEST(VectorTest, CopyAssignment) {
  not_std::Vector<Trivial> v1;
  v1.push_back({1, 1.0f});

  not_std::Vector<Trivial> v2;
  v2 = v1;

  EXPECT_EQ(v2.size(), 1);
  EXPECT_EQ(v2[0].a, 1);
}

TEST(VectorTest, MoveAssignment) {
  not_std::Vector<Trivial> v1;
  v1.push_back({1, 1.0f});

  not_std::Vector<Trivial> v2;
  v2 = std::move(v1);

  EXPECT_EQ(v2.size(), 1);
}

TEST(VectorTest, GrowthReallocation) {
  not_std::Vector<Trivial> v;

  for (int i = 0; i < 100; i++)
    v.push_back({i, float(i)});

  EXPECT_EQ(v.size(), 100);

  for (int i = 0; i < 100; i++)
    EXPECT_EQ(v[i].a, i);
}

