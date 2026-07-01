#include <gtest/gtest.h>
#include "List.hpp"


TEST(List, FrontBack) {
  not_std::List<int> x;
  x.push_back(100);
  x.push_back(1);
  x.push_back(88);
  EXPECT_EQ(x.front(), 100);
  EXPECT_EQ(x.back(), 88);
}
TEST(List, PushFront) {
  not_std::List<char> x;
  x.push_back('a');
  x.push_back('b');
  x.push_back('c');
  x.push_front('z');
  EXPECT_EQ(x.front(), 'z');
  EXPECT_EQ(x.back(), 'c');
  EXPECT_EQ(x.size(), 4);
}

TEST(List, PopFrontPopBack) {
  not_std::List<int> x;
  x.push_back(100);
  x.push_back(1);
  x.push_back(88);

  x.pop_front();
  x.pop_back();
  EXPECT_EQ(x.size(), 1);
  EXPECT_EQ(x.front(), 1);
  EXPECT_EQ(x.back(), 1);

}

TEST(List, Swap) {
  not_std::List<int> first;
  first.push_back(100);
  first.push_back(100);
  not_std::List<int> second;
  second.push_back(200);
  second.push_back(200);
  first.swap(second);
  EXPECT_EQ(first.size(), 2);
  EXPECT_EQ(second.size(), 3);

  EXPECT_EQ(first.front(), 200);
  EXPECT_EQ(first.back(), 200);

  EXPECT_EQ(second.front(), 100);
  EXPECT_EQ(second.back(), 100);
}

