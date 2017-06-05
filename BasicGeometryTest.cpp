#include "gtest/gtest.h"

#include "BasicGeometry.hpp"

#include <limits>

namespace geoIndex {

TEST(GenericPoint, FloatPoint) {
  // This is here just to be sure the template instantiates.
  FloatPoint withFloats{4.25, 23.3, 51};
  withFloats.x = 0;
  ASSERT_EQ(0, withFloats.x); // Here just to avoid "variable defined but not used".
}
  

TEST(SquaredDistance, samePoint) {
  Point p{1, 2, 3};
  ASSERT_EQ(0, SquaredDistance(p, p));
}

TEST(SquaredDistance, unitApart) {
  Point p1{0, 0, 0};
  Point p2{1, 0, 0};
  ASSERT_EQ(1, SquaredDistance(p1, p2));
}

TEST(SquaredDistance, randomPoints) {
  Point p1{4.25, 23.3, 51};
  Point p2{1, 4, 4};
  ASSERT_DOUBLE_EQ(2592.0524999999998, SquaredDistance(p1, p2));
}

#ifdef GEO_INDEX_SAFETY_CHECKS

TEST(SquaredDistance, overflow) {
  Point p1{std::numeric_limits<double>::max(), -9999999999, -9999999999};
  Point p2{std::numeric_limits<double>::min(), +9999999999, +9999999999};
  ASSERT_ANY_THROW(SquaredDistance(p1, p2));
}

#endif

}