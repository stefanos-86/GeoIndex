#include "gtest/gtest.h"

#include "Common.hpp"

#include <algorithm>
#include <vector>

#include "BasicGeometry.hpp"

namespace geoIndex {
  
TEST(IndexAndDistance, operators) {
  const IndexAndDistance<Point> big{4, 10000000};
  const IndexAndDistance<Point> small{4, 0.00001};
  
  ASSERT_GT(big, small);
  ASSERT_LT(small, big);
  
  ASSERT_LE(small, small);
  ASSERT_GE(small, small);
}
  
TEST(IndexAndDistance, sort) {
  const IndexAndDistance<Point> first{4, 1.0};  // Index intentioally out of order, to ensure sorting is on distance.
  const IndexAndDistance<Point> second{56, 2.0};
  const IndexAndDistance<Point> third{2, 3.0};
  
  std::vector<IndexAndDistance<Point> > pointsToSort;
  pointsToSort.push_back(third);
  pointsToSort.push_back(second);
  pointsToSort.push_back(first);
  
  std::sort(begin(pointsToSort), end(pointsToSort));
  
  ASSERT_EQ(4, pointsToSort.at(0).pointIndex);
}

}