#include "gtest/gtest.h"

#include "AabbIndex.hpp"

#include "TestsForAllIndexes.hpp"

namespace geoIndex {

static const int expectedIndexSize = 5; // Anything would do. We don't care about performance here.
    
TEST(AabbIndex, pointsWithinDistance_samePoint) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_samePoint(index);
}

TEST(AabbIndex, pointsWithinDistance_coincidentPoints) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_coincidentPoints(index);
}

TEST(AabbIndex, pointsWithinDistance_noPoints) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_noPoints(index);
}

TEST(AabbIndex, pointsWithinDistance_onlyFarPoints) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_onlyFarPoints(index);
}

TEST(AabbIndex, pointsWithinDistance_inAndOutPoints) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_inAndOutPoints(index);
}

TEST(AabbIndex, pointsWithinDistance_exactDistance) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_exactDistance(index);
}

TEST(AabbIndex, pointsWithinDistance_outputOrder) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_outputOrder(index);
}

TEST(AabbIndex, pointsWithinDistance_squareDistance) {
    AabbIndex<Point> index;
    pointsWithinDistance_squareDistance(index);
}


#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(AabbIndex, index_duplicatedIndex) {
    AabbIndex<Point> index(expectedIndexSize);
    index_duplicatedIndex(index);
}

TEST(AabbIndex, pointsWithinDistance_negativeDistance) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_negativeDistance(index);
}

TEST(AabbIndex, pointsWithinDistance_zeroDistance) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_zeroDistance(index);
}

TEST(AabbIndex, pointsWithinDistance_NanDistance) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_NanDistance(index);
}

TEST(AabbIndex, pointsWithinDistance_overflowDistance) {
    AabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_overflowDistance(index);
}


#endif

/* Specific tests for this algorithm only. */
TEST(AabbIndex, pointsWithinDistance_aabbBoundaries) {
    const double approxSqrtOf2 = 1.414;
    const Point inCorner{approxSqrtOf2, approxSqrtOf2, 1.414};  
    const Point centerOfTopFace{0, 0, 1};
    const Point origin{0, 0, 0};
    const double searchDistance = 1.01;  // The search is strictly inside the distance, need to go "a bit farther" to find centerOfTopFace.
    
    AabbIndex<Point> gi;
    gi.index(inCorner, 1);
    gi.index(centerOfTopFace, 2);
    gi.completed();
    
    std::vector<IndexAndSquaredDistance<Point>> result;
    gi.pointsWithinDistance(origin, searchDistance, result);
    ASSERT_EQ(1, result.size());
    ASSERT_INDEX_PRESENT(result, 2);
}
    
#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(AabbIndex, pointsWithinDistance_incorrectOrderOfUsage_lookupOfNothing) {
    const Point anyPoint{1, 55, 2};
  
    AabbIndex<Point> gi;
    
    std::vector<IndexAndSquaredDistance<Point>> result;
    ASSERT_NO_THROW(gi.pointsWithinDistance(anyPoint, 0.01, result));
}

TEST(AabbIndex, pointsWithinDistance_incorrectOrderOfUsage_lookupWithoutPreparation) {
    const Point anyPoint{1, 55, 2};
  
    AabbIndex<Point> gi;
    gi.index(anyPoint, 1);
    // No call to completed();
    
    std::vector<IndexAndSquaredDistance<Point>> result;
    ASSERT_ANY_THROW(gi.pointsWithinDistance(anyPoint, 0.01, result));
}

TEST(AabbIndex, pointsWithinDistance_incorrectOrderOfUsage_addPointsAfterCompletition) {
    const Point anyPoint{1, 55, 2};
  
    AabbIndex<Point> gi;
    gi.index(anyPoint, 1);
    gi.completed();
    gi.index(anyPoint, 2);
    
    std::vector<IndexAndSquaredDistance<Point>> result;
    ASSERT_ANY_THROW(gi.pointsWithinDistance(anyPoint, 0.01, result));
}
#endif

}