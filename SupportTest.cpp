#include "gtest/gtest.h"

#include "Support.hpp"

using namespace geoIndex;

TEST(Distance, samePoint) {
  Point p{1, 2, 3};
  ASSERT_EQ(0, Distance(p, p));
}

TEST(Distance, unitApart) {
  Point p1{0, 0, 0};
  Point p2{1, 0, 0};
  ASSERT_EQ(1, Distance(p1, p2));
}

TEST(Distance, randomPoints) {
  Point p1{4.25, 23.3, 51};
  Point p2{1, 4, 4};
  ASSERT_DOUBLE_EQ(50.912203841515243, Distance(p1, p2));
}