#include "gtest/gtest.h"

#include "CubeIndex.hpp"

#include <vector>

namespace geoIndex {

TEST(CubeCollection, origin) {
  CubeCollection cc;
  cc.insert(0, 0, 0, 10);

  ASSERT_EQ(std::vector<PointIndex>{10}, cc.read(0, 0, 0));
}

TEST(CubeCollection, readFromUnmappedSlice) {
  CubeCollection cc;
  ASSERT_EQ(std::vector<PointIndex>{}, cc.read(0, 0, 0));
}

TEST(CubeCollection, readFromUnmappedRow) {
  CubeCollection cc;
  cc.insert(0, 0, 0, 0);
  ASSERT_EQ(std::vector<PointIndex>{}, cc.read(0, 1, 0));
}

TEST(CubeCollection, readFromUnmappedCube) {
  CubeCollection cc;
  cc.insert(0, 0, 0, 0);
  cc.insert(0, 1, 0, 1);
  ASSERT_EQ(std::vector<PointIndex>{}, cc.read(0, 1, 2));
}

TEST(CubeIndex, onePoint) {
  CubeIndex cu(10);
  const Point p{0, 0, 0};
  
  cu.addOnePoint(p, 34);
  std::vector<PointIndex> closePoints;
  cu.closeTo(p, closePoints);
  
  ASSERT_EQ(std::vector<PointIndex>{34}, closePoints);
}

TEST(CubeIndex, negativeCoords) {
  CubeIndex cu(10);
  const Point p{0, -10, 0};
  
  cu.addOnePoint(p, 34);
  std::vector<PointIndex> closePoints;
  cu.closeTo(p, closePoints);
  
  ASSERT_EQ(std::vector<PointIndex>{34}, closePoints);
}


TEST(CubeIndex, closeToLimits) {
  CubeIndex cu(10);
  const Point red{0, 9, 0};
  const Point green{0, 11, 0};
  
  cu.addOnePoint(red, 34);
  std::vector<PointIndex> closePoints;
  cu.closeTo(green, closePoints);
  
  ASSERT_EQ(std::vector<PointIndex>{34}, closePoints);
}

// Testare i negativi!
}