#include "gtest/gtest.h"

#include "NoIndex.hpp"

#include "Common.hpp"
#include "TestsForAllIndexes.hpp"

using namespace std;

namespace geoIndex {


TEST(NoIndex, pointsWithinDistance_samePoint) {
    pointsWithinDistance_samePoint<NoIndex<Point>>();
}

TEST(NoIndex, pointsWithinDistance_coincidentPoints) {
    pointsWithinDistance_coincidentPoints<NoIndex<Point>>();
}

TEST(NoIndex, pointsWithinDistance_noPoints) {
  pointsWithinDistance_noPoints<NoIndex<Point>>();
}

TEST(NoIndex, pointsWithinDistance_onlyFarPoints) {
    pointsWithinDistance_onlyFarPoints<NoIndex<Point>>();
}

TEST(NoIndex, pointsWithinDistance_inAndOutPoints) {
    pointsWithinDistance_inAndOutPoints<NoIndex<Point>>();
}

TEST(NoIndex, pointsWithinDistance_exactDistance) {
    pointsWithinDistance_exactDistance<NoIndex<Point>>();
}

TEST(NoIndex, pointsWithinDistance_outputOrder) {
    pointsWithinDistance_outputOrder<NoIndex<Point>>();
}


#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(NoIndex, index_duplicatedIndex) {
  index_duplicatedIndex<NoIndex<Point>>();
}

TEST(NoIndex, pointsWithinDistance_negativeDistance) {
    pointsWithinDistance_negativeDistance<NoIndex<Point>>();
}

TEST(NoIndex, pointsWithinDistance_zeroDistance) {
    pointsWithinDistance_zeroDistance<NoIndex<Point>>();
}

TEST(NoIndex, pointsWithinDistance_NanDistance) {
    pointsWithinDistance_NanDistance<NoIndex<Point>>();
}

TEST(NoIndex, pointsWithinDistance_overflowDistance) {
    pointsWithinDistance_overflowDistance<NoIndex<Point>>();
}

#endif

}