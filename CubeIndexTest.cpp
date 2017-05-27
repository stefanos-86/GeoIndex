#include "gtest/gtest.h"

#include "CubeIndex.hpp"

#include <vector>

#include "Common.hpp"
#include "TestsForAllIndexes.hpp"

using namespace std;

namespace geoIndex {
  

TEST(CubeIndex, pointsWithinDistance_samePoint) {
    pointsWithinDistance_samePoint<CubeIndex<Point>>();
}

TEST(CubeIndex, pointsWithinDistance_coincidentPoints) {
    pointsWithinDistance_coincidentPoints<CubeIndex<Point>>();
}

TEST(CubeIndex, pointsWithinDistance_noPoints) {
  pointsWithinDistance_noPoints<CubeIndex<Point>>();
}

TEST(CubeIndex, pointsWithinDistance_onlyFarPoints) {
    pointsWithinDistance_onlyFarPoints<CubeIndex<Point>>();
}

TEST(CubeIndex, pointsWithinDistance_inAndOutPoints) {
    pointsWithinDistance_inAndOutPoints<CubeIndex<Point>>();
}

TEST(CubeIndex, pointsWithinDistance_exactDistance) {
    pointsWithinDistance_exactDistance<CubeIndex<Point>>();
}

TEST(CubeIndex, pointsWithinDistance_outputOrder) {
    pointsWithinDistance_outputOrder<CubeIndex<Point>>();
}


#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(CubeIndex, index_duplicatedIndex) {
  index_duplicatedIndex<CubeIndex<Point>>();
}

TEST(CubeIndex, pointsWithinDistance_negativeDistance) {
    pointsWithinDistance_negativeDistance<CubeIndex<Point>>();
}

TEST(CubeIndex, pointsWithinDistance_zeroDistance) {
    pointsWithinDistance_zeroDistance<CubeIndex<Point>>();
}

TEST(CubeIndex, pointsWithinDistance_NanDistance) {
    pointsWithinDistance_NanDistance<CubeIndex<Point>>();
}

TEST(CubeIndex, pointsWithinDistance_overflowDistance) {
    pointsWithinDistance_overflowDistance<CubeIndex<Point>>();
}


/* Specific tests for this implementation. */
TEST(CubeCollection, origin) {
    CubeCollection<Point> cc;
    cc.insert(0, 0, 0, 10);

    ASSERT_EQ(std::vector<Point::index_t>{10}, cc.read(0, 0, 0));
}


TEST(CubeCollection, readFromUnmappedSlice) {
    CubeCollection<Point> cc;
    ASSERT_EQ(std::vector<Point::index_t>{}, cc.read(0, 0, 0));
}
 
TEST(CubeCollection, readFromUnmappedRow) {
    CubeCollection<Point> cc;
    cc.insert(0, 0, 0, 0);
    ASSERT_EQ(std::vector<Point::index_t>{}, cc.read(0, 1, 0));
}
 
TEST(CubeCollection, readFromUnmappedCube) {
    CubeCollection<Point> cc;
    cc.insert(0, 0, 0, 0);
    cc.insert(0, 1, 0, 1);
    ASSERT_EQ(std::vector<Point::index_t>{}, cc.read(0, 1, 2));
}

// TODO: Limit cases with points near the cube limits.
// TODO: different cube steps, different distance with respect to the cubes.

#endif

}