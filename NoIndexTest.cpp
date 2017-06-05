#include "gtest/gtest.h"

#include "NoIndex.hpp"

#include "Common.hpp"
#include "TestsForAllIndexes.hpp"

using namespace std;

namespace geoIndex {


TEST(NoIndex, pointsWithinDistance_samePoint) {
    NoIndex<Point> index;
    pointsWithinDistance_samePoint(index);
}

TEST(NoIndex, pointsWithinDistance_coincidentPoints) {
    NoIndex<Point> index;
    pointsWithinDistance_coincidentPoints(index);
}

TEST(NoIndex, pointsWithinDistance_noPoints) {
    NoIndex<Point> index;
    pointsWithinDistance_noPoints(index);
}

TEST(NoIndex, pointsWithinDistance_onlyFarPoints) {
    NoIndex<Point> index;
    pointsWithinDistance_onlyFarPoints(index);
}

TEST(NoIndex, pointsWithinDistance_inAndOutPoints) {
    NoIndex<Point> index;
    pointsWithinDistance_inAndOutPoints(index);
}

TEST(NoIndex, pointsWithinDistance_exactDistance) {
    NoIndex<Point> index;
    pointsWithinDistance_exactDistance(index);
}

TEST(NoIndex, pointsWithinDistance_outputOrder) {
    NoIndex<Point> index;
    pointsWithinDistance_outputOrder(index);
}

TEST(NoIndex, pointsWithinDistance_squareDistance) {
    NoIndex<Point> index;
    pointsWithinDistance_squareDistance(index);
}


#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(NoIndex, index_duplicatedIndex) {
    NoIndex<Point> index;
    index_duplicatedIndex(index);
}

TEST(NoIndex, pointsWithinDistance_negativeDistance) {
    NoIndex<Point> index;
    pointsWithinDistance_negativeDistance(index);
}

TEST(NoIndex, pointsWithinDistance_zeroDistance) {
    NoIndex<Point> index;
    pointsWithinDistance_zeroDistance(index);
}

TEST(NoIndex, pointsWithinDistance_NanDistance) {
    NoIndex<Point> index;
    pointsWithinDistance_NanDistance(index);
}

TEST(NoIndex, pointsWithinDistance_overflowDistance) {
    NoIndex<Point> index;
    pointsWithinDistance_overflowDistance(index);
}

#endif

}