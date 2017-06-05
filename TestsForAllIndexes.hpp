#ifndef GEOINDEX_TESTS_FOR_ALL_INDEXES
#define GEOINDEX_TESTS_FOR_ALL_INDEXES

#include "gtest/gtest.h"

#include "Common.hpp"
#include "DomainAssertions.hpp"

namespace geoIndex {

/** All the indexes must pass the tests defined in this file.
 *  There is no point in duplicating them. Those are the "invariants" that all indexes must respect.
 *
 *  Simply pass each implementation as a parameter and then re-use for the specific tests. 
 *  
 *  Red mesh: in the original problem statemet we had two meshes. The one to search into was "red".
 */


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_samePoint(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{1, 55, 2};
  const PointTraits<Point>::index referenceIndex = 1;
  
  redMesh.index(referencePoint, referenceIndex);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 0.01, result);
  
  ASSERT_INDEX_PRESENT(result, referenceIndex);
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_coincidentPoints(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{1, 55, 2};
  
  redMesh.index(referencePoint, 1);
  redMesh.index(referencePoint, 2);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 0.01, result);
  
  ASSERT_INDEX_PRESENT(result, 1);
  ASSERT_INDEX_PRESENT(result, 2);
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_noPoints(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{0, 0, 0};
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 0.01, result);
  
  ASSERT_TRUE(result.empty());
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_onlyFarPoints(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{0, 0, 0};
  const Point far1{100, 0, 0};
  const Point far2{154, 5, 0};
  const Point far3{100, 0, 256};
  
  redMesh.index(far1, 1);
  redMesh.index(far2, 2);
  redMesh.index(far3, 3);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 0.01, result);
  
  ASSERT_TRUE(result.empty());
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_inAndOutPoints(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{0, 0, 0};
  const Point pIn{99.99, 0, 0};
  const Point pOut{101.1, 0, 0};
  
  redMesh.index(pIn, 1);
  redMesh.index(pOut, 2);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 101, result);
  
  ASSERT_EQ(1, result.size());
  ASSERT_INDEX_PRESENT(result, 1);
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_exactDistance(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{0, 0, 0};
  const Point onTheBorder{100, 0, 0};
  
  redMesh.index(onTheBorder, 1);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 100, result);
  
  ASSERT_TRUE(result.empty());
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_outputOrder(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{0, 0, 0};
  const Point closest{1, 0, 0};
  const Point middle{2, 0, 0};
  const Point far{3, 0, 0};
  
  const PointTraits<Point>::index closestIndex = 1;
  const PointTraits<Point>::index middleIndex = 2;
  const PointTraits<Point>::index farIndex = 3;
  
  redMesh.index(closest, closestIndex);
  redMesh.index(far, farIndex);
  redMesh.index(middle, middleIndex);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 4, result);
  
  ASSERT_EQ(closestIndex, result.at(0).pointIndex);
  ASSERT_EQ(middleIndex, result.at(1).pointIndex);
  ASSERT_EQ(farIndex, result.at(2).pointIndex);
}


/* Safety checks.*/
template <typename GEOMETRY_INDEX>
void index_duplicatedIndex(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{1, 55, 2};
  const PointTraits<Point>::index sameIndex = 1;
  
  redMesh.index(referencePoint, sameIndex);
  
  ASSERT_ANY_THROW(redMesh.index(referencePoint, sameIndex));
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_negativeDistance(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{1, 55, 2};
  const PointTraits<Point>::index index = 1;
  
  redMesh.index(referencePoint, index);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithinDistance(referencePoint, -1, result););
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_zeroDistance(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{1, 55, 2};
  const PointTraits<Point>::index index = 1;
  
  redMesh.index(referencePoint, index);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithinDistance(referencePoint, 0, result););
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_NanDistance(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{1, 55, 2};
  const PointTraits<Point>::index index = 1;
  
  redMesh.index(referencePoint, index);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithinDistance(referencePoint, std::numeric_limits<double>::quiet_NaN(), result););
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_overflowDistance(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{1, 55, 2};
  const PointTraits<Point>::index index = 1;
  
  redMesh.index(referencePoint, index);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithinDistance(referencePoint, std::numeric_limits<double>::max(), result););
}


template <typename GEOMETRY_INDEX>
void pointsWithinDistance_squareDistance(GEOMETRY_INDEX& redMesh) {
  const Point referencePoint{0, 0, 0};
  const Point neighbor{2, 0, 0};
  
  const PointTraits<Point>::index neighborIndex = 1;

  redMesh.index(neighbor, neighborIndex);
  
  std::vector<IndexAndSquaredDistance<Point>> result;
  redMesh.pointsWithinDistance(referencePoint, 4, result);
  
  ASSERT_EQ(4, result.at(0).geometricValue);
}

}

#endif