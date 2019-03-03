#include "gtest/gtest.h"

#include "BoostIndex.hpp"

#include "Common.hpp"
#include "TestsForAllIndexes.hpp"


using namespace std;

namespace geoIndex {



TEST(BoostIndex, pointsWithinDistance_samePoint) {
    BoostIndex<Point> index;
    pointsWithinDistance_samePoint(index);
}

TEST(BoostIndex, pointsWithinDistance_coincidentPoints) {
    BoostIndex<Point> index;
    pointsWithinDistance_coincidentPoints(index);
}

TEST(BoostIndex, pointsWithinDistance_noPoints) {
    BoostIndex<Point> index;
    pointsWithinDistance_noPoints(index);
}

TEST(BoostIndex, pointsWithinDistance_onlyFarPoints) {
    BoostIndex<Point> index;
    pointsWithinDistance_onlyFarPoints(index);
}

TEST(BoostIndex, pointsWithinDistance_inAndOutPoints) {
    BoostIndex<Point> index;
    pointsWithinDistance_inAndOutPoints(index);
}

TEST(BoostIndex, pointsWithinDistance_exactDistance) {
    BoostIndex<Point> index;
    pointsWithinDistance_exactDistance(index);
}

TEST(BoostIndex, pointsWithinDistance_outputOrder) {
    BoostIndex<Point> index;
    pointsWithinDistance_outputOrder(index);
}

TEST(BoostIndex, pointsWithinDistance_squareDistance) {
    BoostIndex<Point> index;
    pointsWithinDistance_squareDistance(index);
}


#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(BoostIndex, index_duplicatedIndex) {
    BoostIndex<Point> index;
    index_duplicatedIndex(index);
}

TEST(BoostIndex, pointsWithinDistance_negativeDistance) {
    BoostIndex<Point> index;
    pointsWithinDistance_negativeDistance(index);
}

TEST(BoostIndex, pointsWithinDistance_zeroDistance) {
    BoostIndex<Point> index;
    pointsWithinDistance_zeroDistance(index);
}

TEST(BoostIndex, pointsWithinDistance_NanDistance) {
    BoostIndex<Point> index;
    pointsWithinDistance_NanDistance(index);
}

TEST(BoostIndex, pointsWithinDistance_overflowDistance) {
    BoostIndex<Point> index;
    pointsWithinDistance_overflowDistance(index);
}

#endif

}
