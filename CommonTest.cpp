#include "gtest/gtest.h"

#include "Common.hpp"

#include <algorithm>
#include <vector>

#include "BasicGeometry.hpp"

namespace geoIndex {
  
TEST(IndexAndGeometry, SortByGeometry) {
    const IndexAndGeometry<Point> first{4, 1.0};  // Index intentionally out of order, to ensure sorting is on distance.
    const IndexAndGeometry<Point> second{56, 2.0};
    const IndexAndGeometry<Point> third{2, 3.0};

    std::vector<IndexAndGeometry<Point> > pointsToSort;
    pointsToSort.push_back(third);
    pointsToSort.push_back(second);
    pointsToSort.push_back(first);

    std::sort(begin(pointsToSort), end(pointsToSort), SortByGeometry<Point>);

    ASSERT_EQ(4, pointsToSort.at(0).pointIndex);
    ASSERT_EQ(56, pointsToSort.at(1).pointIndex);
    ASSERT_EQ(2, pointsToSort.at(2).pointIndex);
}


TEST(IndexAndGeometry, SortByPointIndex) {
    const IndexAndGeometry<Point> first{4, 10000.0};  // Geometry intentionally out of order, to ensure sorting is on distance.
    const IndexAndGeometry<Point> second{56, 2.0};
    const IndexAndGeometry<Point> third{200, -3.0};

    std::vector<IndexAndGeometry<Point> > pointsToSort;
    pointsToSort.push_back(third);
    pointsToSort.push_back(second);
    pointsToSort.push_back(first);

    std::sort(begin(pointsToSort), end(pointsToSort), SortByPointIndex<Point>);

    ASSERT_EQ(4, pointsToSort.at(0).pointIndex);
    ASSERT_EQ(56, pointsToSort.at(1).pointIndex);
    ASSERT_EQ(200, pointsToSort.at(2).pointIndex);
}

}