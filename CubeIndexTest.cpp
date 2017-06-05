#include "gtest/gtest.h"

#include "CubeIndex.hpp"

#include <vector>
#include <limits>
#include "Common.hpp"
#include "TestsForAllIndexes.hpp"

using namespace std;

namespace geoIndex {

static const PointTraits<Point>::coordinate gridStep = 10.0; // More to illustrate how to use it than other reasons.

TEST(CubeIndex, pointsWithinDistance_samePoint) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_samePoint(index);
}

TEST(CubeIndex, pointsWithinDistance_coincidentPoints) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_coincidentPoints(index);
}

TEST(CubeIndex, pointsWithinDistance_noPoints) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_noPoints(index);
}

TEST(CubeIndex, pointsWithinDistance_onlyFarPoints) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_onlyFarPoints(index);
}

TEST(CubeIndex, pointsWithinDistance_inAndOutPoints) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_inAndOutPoints(index);
}

TEST(CubeIndex, pointsWithinDistance_exactDistance) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_exactDistance(index);
}

TEST(CubeIndex, pointsWithinDistance_outputOrder) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_outputOrder(index);
}


#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(CubeIndex, index_duplicatedIndex) {
    CubeIndex<Point> index(gridStep);
    index_duplicatedIndex(index);
}

TEST(CubeIndex, pointsWithinDistance_negativeDistance) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_negativeDistance(index);
}

TEST(CubeIndex, pointsWithinDistance_zeroDistance) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_zeroDistance(index);
}

TEST(CubeIndex, pointsWithinDistance_NanDistance) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_NanDistance(index);
}

TEST(CubeIndex, pointsWithinDistance_overflowDistance) {
    CubeIndex<Point> index(gridStep);
    pointsWithinDistance_overflowDistance(index);
}

#endif


/* Specific tests for this implementation. */
TEST(CubeCollection, origin) {
    CubeCollection<Point> cc;
    cc.insert(0, 0, 0, 10);

    ASSERT_EQ(std::vector<PointTraits<Point>::index>{10}, cc.read(0, 0, 0));
}


TEST(CubeCollection, readFromUnmappedSlice) {
    CubeCollection<Point> cc;
    ASSERT_EQ(std::vector<PointTraits<Point>::index>{}, cc.read(0, 0, 0));
}
 
TEST(CubeCollection, readFromUnmappedRow) {
    CubeCollection<Point> cc;
    cc.insert(0, 0, 0, 0);
    ASSERT_EQ(std::vector<PointTraits<Point>::index>{}, cc.read(0, 1, 0));
}
 
TEST(CubeCollection, readFromUnmappedCube) {
    CubeCollection<Point> cc;
    cc.insert(0, 0, 0, 0);
    cc.insert(0, 1, 0, 1);
    ASSERT_EQ(std::vector<PointTraits<Point>::index>{}, cc.read(0, 1, 2));
}

#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(CubeIndex, index_cubicCoordinateOverflow) {
    const double probablyBiggerThanCubicCoordinate = static_cast<double>(std::numeric_limits<CubicCoordinate>::max()) + 1.0;
    CubeIndex<Point> cu(0.1); // This pushes things farther.
    ASSERT_ANY_THROW(cu.index(Point{probablyBiggerThanCubicCoordinate, 1.0, 1},  1));
}

TEST(CubeIndex, pointsWithinDistance_scanDistanceOverflow) {
    const double limitOfCoordinates = static_cast<double>(std::numeric_limits<CubicCoordinate>::max());
    CubeIndex<Point> cu(1);
    std::vector<IndexAndSquaredDistance<Point> > output;
    ASSERT_ANY_THROW(cu.pointsWithinDistance(Point{0, 0, 0}, limitOfCoordinates, output));
}

TEST(CubeIndex, pointsWithinDistance_scanPlusReference) {
    const double referenceCloseToLimit = static_cast<double>(std::numeric_limits<CubicCoordinate>::max() - 10);
    CubeIndex<Point> cu(1);
    std::vector<IndexAndSquaredDistance<Point> > output;
    ASSERT_ANY_THROW(cu.pointsWithinDistance(Point{referenceCloseToLimit, 0, 0}, 20, output));
}

// TODO: Limit cases with points near the cube limits.
// TODO: different cube steps, different distance with respect to the cubes, idiotic cube sizes.

#endif

}