#include "gtest/gtest.h"

#include "Support.hpp"

TEST(Point, creation) {
  Point p{1, 2, 3};
  ASSERT_EQ(1, p.x);
}