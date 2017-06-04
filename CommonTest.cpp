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

#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(CheckOverflow, NoOverflow) {
    ASSERT_NO_THROW(CheckOverflow<double>(29));
}

TEST(CheckOverflow, Overflow) {
    ASSERT_ANY_THROW(CheckOverflow<double>(1.0 / 0.0));
}

TEST(CheckMeaningfulCullingDistance, correct) {
    ASSERT_NO_THROW(CheckMeaningfulCullingDistance<float>(29));
}

TEST(CheckMeaningfulCullingDistance, notADistance) {
    ASSERT_ANY_THROW(CheckMeaningfulCullingDistance<float>(-29));
}

TEST(CheckMeaningfulCullingDistance, insane) {
    ASSERT_ANY_THROW(CheckMeaningfulCullingDistance<float>(0.0 / 0.0));
}


TEST(StopSumOverflow, NoOverflow) {
    ASSERT_NO_THROW(StopSumOverflow<int>(1, 1));
}

TEST(StopSumOverflow, YesOverflow) {
    ASSERT_ANY_THROW(StopSumOverflow<uint8_t>(254, 10));
}

TEST(StopSumOverflow, Negatives) {
    ASSERT_NO_THROW(StopSumOverflow<int8_t>(127, -1));
}

TEST(StopDifferenceUnderflow, Ok) {
    ASSERT_NO_THROW(StopDifferenceUnderflow<int>(1, 1));
}

TEST(StopDifferenceUnderflow, BelowMin) {
    ASSERT_ANY_THROW(StopDifferenceUnderflow<uint8_t>(10, 11));
}

TEST(StopDifferenceUnderflow, Negatives) {
    ASSERT_NO_THROW(StopDifferenceUnderflow<int8_t>(1, -2));
}
#endif

}