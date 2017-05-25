#include "gtest/gtest.h"

#include "AabbIndex.hpp"

#include "TestsForAllIndexes.hpp"

namespace geoIndex {
    
TEST(AabbIndex, pointsWithinDistance_samePoint) {
    pointsWithinDistance_samePoint<AabbIndex<Point>>();
}

TEST(AabbIndex, pointsWithinDistance_coincidentPoints) {
    pointsWithinDistance_coincidentPoints<AabbIndex<Point>>();
}

TEST(AabbIndex, pointsWithinDistance_noPoints) {
  pointsWithinDistance_noPoints<AabbIndex<Point>>();
}

TEST(AabbIndex, pointsWithinDistance_onlyFarPoints) {
    pointsWithinDistance_onlyFarPoints<AabbIndex<Point>>();
}

TEST(AabbIndex, pointsWithinDistance_inAndOutPoints) {
    pointsWithinDistance_inAndOutPoints<AabbIndex<Point>>();
}

TEST(AabbIndex, pointsWithinDistance_exactDistance) {
    pointsWithinDistance_exactDistance<AabbIndex<Point>>();
}

TEST(AabbIndex, pointsWithinDistance_outputOrder) {
    pointsWithinDistance_outputOrder<AabbIndex<Point>>();
}


#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(AabbIndex, index_duplicatedIndex) {
  index_duplicatedIndex<AabbIndex<Point>>();
}

TEST(AabbIndex, pointsWithinDistance_negativeDistance) {
    pointsWithinDistance_negativeDistance<AabbIndex<Point>>();
}

TEST(AabbIndex, pointsWithinDistance_zeroDistance) {
    pointsWithinDistance_zeroDistance<AabbIndex<Point>>();
}

TEST(AabbIndex, pointsWithinDistance_NanDistance) {
    pointsWithinDistance_NanDistance<AabbIndex<Point>>();
}

TEST(AabbIndex, pointsWithinDistance_overflowDistance) {
    pointsWithinDistance_overflowDistance<AabbIndex<Point>>();
}

//TODO: test che chiamano i lookup prima di chiudere l'indice o lo modificano dopo. >> ci deve essere un warning...
// TODO: test con punti negli angoli
#endif


/*
TEST(GeometryIndex_lookup, onePoint) {
  GeometryIndex gi;
  gi.index({0, 0, 0}, 0);
  gi.prepareForLookups();
  
  std::vector<size_t> result;
  gi.withinAabb({0, 0, 0}, 1, result);
  
  ASSERT_EQ(1, result.size());
}

TEST(GeometryIndex_lookup, onePointTooFar) {
  GeometryIndex gi;
  gi.index({0, 0, 0}, 0);
  gi.prepareForLookups();
  
  std::vector<size_t> result;
  gi.withinAabb({1000, 0, 0}, 1, result);
  
  ASSERT_EQ(0, result.size());
}


TEST(GeometryIndex_lookup, morePointsInTheAabb) {
  GeometryIndex gi;
  gi.index({0, 0, 0}, 0);
  gi.index({1, 0, 0}, 1);
  gi.index({0, 1, 0}, 2);
  gi.index({0, 0, 1}, 3);
  gi.prepareForLookups();
  
  std::vector<size_t> result;
  gi.withinAabb({0, 0, 0}, 1.5, result);
  
  ASSERT_EQ(4, result.size());
  ASSERT_NE(std::find(begin(result), end(result), 0), end(result));
  ASSERT_NE(std::find(begin(result), end(result), 1), end(result));
  ASSERT_NE(std::find(begin(result), end(result), 2), end(result));
  ASSERT_NE(std::find(begin(result), end(result), 3), end(result));
}

TEST(GeometryIndex_lookup, pointsOutsideAndInside) {
  GeometryIndex gi;
  gi.index({0, 0, 0}, 0);
  gi.index({1, 0, 0}, 1);
  gi.index({0, 1, 0}, 2);
  gi.index({0, 0, 1}, 3);
  gi.prepareForLookups();
  
  std::vector<size_t> result;
  gi.withinAabb({0.5, 0, 0}, 0.6, result);
  
  ASSERT_EQ(2, result.size());
  ASSERT_NE(std::find(begin(result), end(result), 0), end(result));
  ASSERT_NE(std::find(begin(result), end(result), 1), end(result));
}

*/
}