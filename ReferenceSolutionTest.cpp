#include "gtest/gtest.h"

#include "ReferenceSolution.hpp"

#include <vector>

#include "Support.hpp"

namespace geoIndex {

TEST(TwoClosestPoints, notEnoughPoints) {
  std::vector<Point> referenceGrid = {{1, 2, 3}};
  Point basePoint = {1, 2, 3};
  
  ASSERT_ANY_THROW(TwoClosestPoints(basePoint, referenceGrid));
  
}

TEST(TwoClosestPoints, exactlyTwoPoints) {
  std::vector<Point> referenceGrid = {{1, 2, 3}, {4, 5, 6}};
  Point basePoint = {0, 0, 0};
  
    std::pair<PointIndex, PointIndex> result = TwoClosestPoints(basePoint, referenceGrid);
  ASSERT_EQ(0, result.first);
  ASSERT_EQ(1, result.second);
}


TEST(TwoClosestPoints, exactlyTwoPoints_inverted) {
  std::vector<Point> referenceGrid = {{1000, 2, 3}, {4, 5, 6}};
  Point basePoint = {0, 0, 0};
  
  std::pair<PointIndex, PointIndex> result = TwoClosestPoints(basePoint, referenceGrid);
  ASSERT_EQ(1, result.first);
  ASSERT_EQ(0, result.second);
}


TEST(TwoClosestPoints, sameDistancePoints) {
  std::vector<Point> referenceGrid = {{1, 0, 0}, {2, 0, 0}, {3, 0, 0}};
  Point basePoint = {2, 1, 0};
  
  
  std::pair<PointIndex, PointIndex> result = TwoClosestPoints(basePoint, referenceGrid);
  ASSERT_EQ(1, result.first);
  ASSERT_EQ(0, result.second);  // 0 or 2 are both the same.
}

TEST(TwoClosestPoints, firstPointSecondDistance) {
  std::vector<Point> referenceGrid = {{1, 0, 0}, {2, 0, 0}, {45, 0, 0}};
  Point basePoint = {2, 1, 0};
  
  
  std::pair<PointIndex, PointIndex> result = TwoClosestPoints(basePoint, referenceGrid);
  ASSERT_EQ(1, result.first);
  ASSERT_EQ(0, result.second);
}



}