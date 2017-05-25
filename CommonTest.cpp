#include "gtest/gtest.h"

#include "Common.hpp"

#include <algorithm>
#include <vector>

#include "BasicGeometry.hpp"

namespace geoIndex {
  
TEST(IndexAndGeometry, operators) {
  const IndexAndGeometry<Point> big{4, 10000000};
  const IndexAndGeometry<Point> small{4, 0.00001};
  
  ASSERT_GT(big, small);
  ASSERT_LT(small, big);
  
  ASSERT_LE(small, small);
  ASSERT_GE(small, small);
}
  
TEST(IndexAndGeometry, sort) {
  const IndexAndGeometry<Point> first{4, 1.0};  // Index intentioally out of order, to ensure sorting is on distance.
  const IndexAndGeometry<Point> second{56, 2.0};
  const IndexAndGeometry<Point> third{2, 3.0};
  
  std::vector<IndexAndGeometry<Point> > pointsToSort;
  pointsToSort.push_back(third);
  pointsToSort.push_back(second);
  pointsToSort.push_back(first);
  
  std::sort(begin(pointsToSort), end(pointsToSort));
  
  ASSERT_EQ(4, pointsToSort.at(0).pointIndex);
}

}