#include "gtest/gtest.h"

#include "NearestNeighbors.hpp"

#include <vector>

#include "BasicGeometry.hpp"
#include "Common.hpp"

#include "NoIndex.hpp"
#include "AabbIndex.hpp"
#include "CubeIndex.hpp"

#include "DomainAssertions.hpp"

using namespace std;

namespace geoIndex {

    
TEST(BuildIndex, NoIndex) {
    std::vector<Point> points;
    const Point testPoint({1, 2, 3});
    points.push_back(testPoint);
    
    NoIndex<Point> indexToBuild(points.size());  // The optimization parameter is irrelevant for the test,
                                                 // but it shows the correct usage.
    BuildIndex(points, indexToBuild);
    
    std::vector<IndexAndSquaredDistance<Point> > readBackThePoint;
    indexToBuild.pointsWithinDistance(testPoint, 150, readBackThePoint);
    
    ASSERT_FALSE(readBackThePoint.empty());
}


TEST(BuildIndex, AabbIndex) {
    std::vector<Point> points;
    const Point testPoint({1, 2, 3});
    points.push_back(testPoint);
    
    AabbIndex<Point> indexToBuild(points.size());  // The optimization parameter is irrelevant for the test,
                                                   // but it shows the correct usage.
    BuildIndex(points, indexToBuild);
    
    std::vector<IndexAndSquaredDistance<Point> > readBackThePoint;
    indexToBuild.pointsWithinDistance(testPoint, 150, readBackThePoint);
    
    ASSERT_FALSE(readBackThePoint.empty());
}

TEST(BuildIndex, CubeIndex) {
    std::vector<Point> points;
    const Point testPoint({1, 2, 3});
    points.push_back(testPoint);
    
    CubeIndex<Point> indexToBuild(10.0);  // The optimization parameter is irrelevant for the test,
                                          // but it shows the correct usage.
    BuildIndex(points, indexToBuild);
    
    std::vector<IndexAndSquaredDistance<Point> > readBackThePoint;
    indexToBuild.pointsWithinDistance(testPoint, 150, readBackThePoint);
    
    ASSERT_FALSE(readBackThePoint.empty());
}


/* Shortcuts some constant patameters*/
static void TestKNearest_NoIndex(const std::vector<Point>& points,
                                 std::vector<IndexAndSquaredDistance<Point> >& result 
) {
    NoIndex<Point> geometryIndex;
    BuildIndex(points, geometryIndex);
    const Point referencePoint{0, 0, 0};
    const PointTraits<Point>::coordinate cullingDistance = 65;
    
    const size_t k = 2;
    
    KNearestNeighbor(
        points,
        geometryIndex,
        cullingDistance,
        referencePoint,
        k,
        result);
}

TEST(KNearestNeighbor, lessThanKPoints) {
    std::vector<Point> points;
    points.push_back(Point{0, 0, 0});
    
    std::vector<IndexAndSquaredDistance<Point> > result;
    TestKNearest_NoIndex(points, result);
    
    ASSERT_EQ(1, result.size());
}

TEST(KNearestNeighbor, KPoints) {
    std::vector<Point> points;
    const Point origin{0, 0, 0};
    points.push_back(origin);
    points.push_back(origin);
    
    std::vector<IndexAndSquaredDistance<Point> > result;
    TestKNearest_NoIndex(points, result);
    ASSERT_EQ(2, result.size());
}

TEST(KNearestNeighbor, MoreThanKPoints) {
    std::vector<Point> points;
    const Point origin{0, 0, 0};
    points.push_back(origin);
    points.push_back(origin);
    points.push_back(origin);
    points.push_back(origin);
    
    std::vector<IndexAndSquaredDistance<Point> > result;
    TestKNearest_NoIndex(points, result);
    ASSERT_EQ(2, result.size());
    
    ASSERT_EQ(2, result.size());
}

TEST(KNearestNeighbor, GenericCase) {
    std::vector<Point> points;
    const Point referencePoint{0, 0, 0};
    points.push_back(Point{70, 1, 2});
    points.push_back(Point{11, 1, 2});
    points.push_back(Point{7, 1, 2});
    points.push_back(Point{7, 2, 2});
    points.push_back(Point{7, 2, -225});
    
    NoIndex<Point> geometryIndex;
    BuildIndex(points, geometryIndex);
    
    const PointTraits<Point>::coordinate cullingDistance = 65;
    
    const size_t k = 3;
    std::vector<IndexAndSquaredDistance<Point> > result;
    
    KNearestNeighbor(
        points,
        geometryIndex,
        cullingDistance,
        referencePoint,
        k,
        result);
    
    ASSERT_EQ(3, result.size());
    ASSERT_INDEX_PRESENT(result, 2);
    ASSERT_INDEX_PRESENT(result, 3);
    ASSERT_INDEX_PRESENT(result, 1);
}

// TODO: test with non-default classes.

#ifdef GEO_INDEX_SAFETY_CHECKS

TEST(KNearestNeighbor, zeroDistance) {
    const std::vector<Point> points;
    const NoIndex<Point> geometryIndex;
    const PointTraits<Point>::coordinate cullingDistance = 0;
   
    
    const Point referencePoint{0, 0, 0};
    const size_t k = 4;
    std::vector<IndexAndSquaredDistance<Point> > result;
    
    
    ASSERT_ANY_THROW(
        KNearestNeighbor(
            points,
            geometryIndex,
            cullingDistance,
            referencePoint,
            k,
            result)
    );
}

TEST(KNearestNeighbor, zeroK) {
    const std::vector<Point> points;
    const NoIndex<Point> geometryIndex;
    const PointTraits<Point>::coordinate cullingDistance = 10;
   
    
    const Point referencePoint{0, 0, 0};
    const size_t k = 0;
    std::vector<IndexAndSquaredDistance<Point> > result;
    
    
    ASSERT_ANY_THROW(
        KNearestNeighbor(
            points,
            geometryIndex,
            cullingDistance,
            referencePoint,
            k,
            result)
    );
}

#endif


}