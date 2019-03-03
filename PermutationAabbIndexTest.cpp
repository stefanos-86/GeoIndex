#include "gtest/gtest.h"

#include "PermutationAabbIndex.hpp"

#include "TestsForAllIndexes.hpp"

namespace geoIndex {

static const int expectedIndexSize = 5; // Anything would do. We don't care about performance here.
    
TEST(PermutationAabbIndex, pointsWithinDistance_samePoint) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_samePoint(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_coincidentPoints) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_coincidentPoints(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_noPoints) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_noPoints(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_onlyFarPoints) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_onlyFarPoints(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_inAndOutPoints) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_inAndOutPoints(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_exactDistance) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_exactDistance(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_outputOrder) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_outputOrder(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_squareDistance) {
    PermutationAabbIndex<Point> index;
    pointsWithinDistance_squareDistance(index);
}


#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(PermutationAabbIndex, index_duplicatedIndex) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    index_duplicatedIndex(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_negativeDistance) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_negativeDistance(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_zeroDistance) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_zeroDistance(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_NanDistance) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_NanDistance(index);
}

TEST(PermutationAabbIndex, pointsWithinDistance_overflowDistance) {
    PermutationAabbIndex<Point> index(expectedIndexSize);
    pointsWithinDistance_overflowDistance(index);
}


#endif

/* Specific tests for this algorithm only.  */
TEST(PermutationAabbIndex, pointsWithinDistance_aabbBoundaries) {
    const double approxSqrtOf2 = 1.414;
    const Point inCorner{approxSqrtOf2, approxSqrtOf2, 1.414};  
    const Point centerOfTopFace{0, 0, 1};
    const Point origin{0, 0, 0};
    const double searchDistance = 1.01;  // The search is strictly inside the distance, need to go "a bit farther" to find centerOfTopFace.
    
    PermutationAabbIndex<Point> gi;
    gi.index(inCorner, 1);
    gi.index(centerOfTopFace, 2);
    gi.completed();
    
    std::vector<IndexAndSquaredDistance<Point>> result;
    gi.pointsWithinDistance(origin, searchDistance, result);
    ASSERT_EQ(1, result.size());
    ASSERT_INDEX_PRESENT(result, 2);
}
    
#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(PermutationAabbIndex, pointsWithinDistance_incorrectOrderOfUsage_lookupOfNothing) {
    const Point anyPoint{1, 55, 2};
  
    PermutationAabbIndex<Point> gi;
    
    std::vector<IndexAndSquaredDistance<Point>> result;
    ASSERT_NO_THROW(gi.pointsWithinDistance(anyPoint, 0.01, result));
}

TEST(PermutationAabbIndex, pointsWithinDistance_incorrectOrderOfUsage_lookupWithoutPreparation) {
    const Point anyPoint{1, 55, 2};
  
    PermutationAabbIndex<Point> gi;
    gi.index(anyPoint, 1);
    // No call to completed();
    
    std::vector<IndexAndSquaredDistance<Point>> result;
    ASSERT_ANY_THROW(gi.pointsWithinDistance(anyPoint, 0.01, result));
}

TEST(PermutationAabbIndex, pointsWithinDistance_incorrectOrderOfUsage_addPointsAfterCompletition) {
    const Point anyPoint{1, 55, 2};
  
    PermutationAabbIndex<Point> gi;
    gi.index(anyPoint, 1);
    gi.completed();
    gi.index(anyPoint, 2);
    
    std::vector<IndexAndSquaredDistance<Point>> result;
    ASSERT_ANY_THROW(gi.pointsWithinDistance(anyPoint, 0.01, result));
}
#endif

}
