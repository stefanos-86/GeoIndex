#include "gtest/gtest.h"

#include "NoIndex.hpp"


#include <vector>
#include <limits>

#include "BasicGeometry.hpp"
#include "Common.hpp"
#include "DomainAssertions.hpp"

using namespace std;

namespace geoIndex {


  
TEST(NoIndex, Construction_unknownSize) {
  NoIndex<Point>();  // Ok it it compiles - can't test the internal collection (well, could - with a memory profiler).
}

TEST(NoIndex, Construction_withSize) {
  NoIndex<Point>(25);  // Ok it it compiles - can't test the internal collection (well, could - with a memory profiler).
}

TEST(NoIndex, pointsWithinDistance_samePoint) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t referenceIndex = 1;
  
  NoIndex<Point> redMesh(1);
  redMesh.index(referencePoint, referenceIndex);
  
  std::vector<IndexAndDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 0.01, result);
  
  ASSERT_INDEX_PRESENT(result, referenceIndex);
}

TEST(NoIndex, pointsWithinDistance_coincidentPoints) {
  const Point referencePoint{1, 55, 2};
  
  NoIndex<Point> redMesh(2);
  redMesh.index(referencePoint, 1);
  redMesh.index(referencePoint, 2);
  
  std::vector<IndexAndDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 0.01, result);
  
  ASSERT_INDEX_PRESENT(result, 1);
  ASSERT_INDEX_PRESENT(result, 2);
}

TEST(NoIndex, pointsWithinDistance_noPoints) {
  const Point referencePoint{0, 0, 0};
  
  NoIndex<Point> redMesh(2);
  
  std::vector<IndexAndDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 0.01, result);
  
  ASSERT_TRUE(result.empty());
}

TEST(NoIndex, pointsWithinDistance_onlyFarPoints) {
  const Point referencePoint{0, 0, 0};
  const Point far1{100, 0, 0};
  const Point far2{154, 5, 0};
  const Point far3{100, 0, 256};
  
  NoIndex<Point> redMesh(3);
  redMesh.index(far1, 1);
  redMesh.index(far2, 2);
  redMesh.index(far3, 3);
  
  std::vector<IndexAndDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 0.01, result);
  
  ASSERT_TRUE(result.empty());
}

TEST(NoIndex, pointsWithinDistance_inAndOutPoints) {
  const Point referencePoint{0, 0, 0};
  const Point pIn{99.99, 0, 0};
  const Point pOut{101.1, 0, 0};
  
  NoIndex<Point> redMesh(2);
  redMesh.index(pIn, 1);
  redMesh.index(pOut, 2);
  
  std::vector<IndexAndDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 101, result);
  
  ASSERT_EQ(1, result.size());
  ASSERT_INDEX_PRESENT(result, 1);
}

TEST(NoIndex, pointsWithinDistance_exactDistance) {
  const Point referencePoint{0, 0, 0};
  const Point onTheBorder{100, 0, 0};
  
  NoIndex<Point> redMesh(1);
  redMesh.index(onTheBorder, 1);
  
  std::vector<IndexAndDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 100, result);
  
  ASSERT_TRUE(result.empty());
}

TEST(NoIndex, pointsWithinDistance_outputOrder) {
  const Point referencePoint{0, 0, 0};
  const Point closest{1, 0, 0};
  const Point middle{2, 0, 0};
  const Point far{3, 0, 0};
  
  const Point::index_t closestIndex = 1;
  const Point::index_t middleIndex = 2;
  const Point::index_t farIndex = 3;
  
  NoIndex<Point> redMesh(3);
  redMesh.index(closest, closestIndex);
  redMesh.index(far, farIndex);
  redMesh.index(middle, middleIndex);
  
  std::vector<IndexAndDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 4, result);
  
  ASSERT_EQ(closestIndex, result.at(0).pointIndex);
  ASSERT_EQ(middleIndex, result.at(1).pointIndex);
  ASSERT_EQ(farIndex, result.at(2).pointIndex);
}


#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(NoIndex, index_duplicatedIndex) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t sameIndex = 1;
  
  NoIndex<Point> redMesh(2);
  redMesh.index(referencePoint, sameIndex);
  
  ASSERT_ANY_THROW(redMesh.index(referencePoint, sameIndex));
}

TEST(NoIndex, pointsWithinDistance_negativeDistance) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t index = 1;
  
  NoIndex<Point> redMesh(1);
  redMesh.index(referencePoint, index);
  
  std::vector<IndexAndDistance<Point>> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithinDistance(referencePoint, -1, result););
}

TEST(NoIndex, pointsWithinDistance_zeroDistance) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t index = 1;
  
  NoIndex<Point> redMesh(1);
  redMesh.index(referencePoint, index);
  
  std::vector<IndexAndDistance<Point>> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithinDistance(referencePoint, 0, result););
}

TEST(NoIndex, pointsWithinDistance_NanDistance) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t index = 1;
  
  NoIndex<Point> redMesh(1);
  redMesh.index(referencePoint, index);
  
  std::vector<IndexAndDistance<Point>> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithinDistance(referencePoint, std::numeric_limits<double>::quiet_NaN(), result););
}

TEST(NoIndex, pointsWithinDistance_overflowDistance) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t index = 1;
  
  NoIndex<Point> redMesh(1);
  redMesh.index(referencePoint, index);
  
  std::vector<IndexAndDistance<Point>> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithinDistance(referencePoint, std::numeric_limits<double>::max(), result););
}

#endif

}