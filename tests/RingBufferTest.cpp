#include "RingBuffer.hpp"
#include <array>
#include <gtest/gtest.h>

TEST(RingBuffer, Rb1) {
  std::array<int, 2> res {2, 88};
  not_std::RingBuffer<int, 2> rb;
  rb.push(1);
  rb.push(2);
  rb.push(88);
  EXPECT_EQ(rb.snapshot(), res);
}

TEST(RingBuffer, Rb2) {
  std::array<char, 2> res {'b', 'c'};
  not_std::RingBuffer<char, 2> rb;
  rb.push('a');
  rb.push('b');
  rb.push('c');
  EXPECT_EQ(rb.snapshot(), res);
}

TEST(RingBuffer, Rb3) {
  std::array<char, 2> res {'x', 'y'};
  not_std::RingBuffer<char, 2> rb;
  rb.push('x');
  rb.push('y');
  EXPECT_EQ(rb.snapshot(), res);
  EXPECT_EQ(rb.snapshot(), res);
}

