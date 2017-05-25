#include "gtest/gtest.h"

#include "GeometryIndex.hpp"
#include <algorithm>

namespace geoIndex {

TEST(GeometryIndex_lookup, onePoint) {
  GeometryIndex gi;
  gi.index({0, 0, 0}, 0);
  gi.prepareForLookups();
  
  std::vector<size_t> result;
  gi.withinAabb({0, 0, 0}, 1, result);
  
  ASSERT_EQ(1, result.size());
}

TEST(GeometryIndex_lookup, onePointTooFar) {
  GeometryIndex gi;
  gi.index({0, 0, 0}, 0);
  gi.prepareForLookups();
  
  std::vector<size_t> result;
  gi.withinAabb({1000, 0, 0}, 1, result);
  
  ASSERT_EQ(0, result.size());
}


TEST(GeometryIndex_lookup, morePointsInTheAabb) {
  GeometryIndex gi;
  gi.index({0, 0, 0}, 0);
  gi.index({1, 0, 0}, 1);
  gi.index({0, 1, 0}, 2);
  gi.index({0, 0, 1}, 3);
  gi.prepareForLookups();
  
  std::vector<size_t> result;
  gi.withinAabb({0, 0, 0}, 1.5, result);
  
  ASSERT_EQ(4, result.size());
  ASSERT_NE(std::find(begin(result), end(result), 0), end(result));
  ASSERT_NE(std::find(begin(result), end(result), 1), end(result));
  ASSERT_NE(std::find(begin(result), end(result), 2), end(result));
  ASSERT_NE(std::find(begin(result), end(result), 3), end(result));
}

TEST(GeometryIndex_lookup, pointsOutsideAndInside) {
  GeometryIndex gi;
  gi.index({0, 0, 0}, 0);
  gi.index({1, 0, 0}, 1);
  gi.index({0, 1, 0}, 2);
  gi.index({0, 0, 1}, 3);
  gi.prepareForLookups();
  
  std::vector<size_t> result;
  gi.withinAabb({0.5, 0, 0}, 0.6, result);
  
  ASSERT_EQ(2, result.size());
  ASSERT_NE(std::find(begin(result), end(result), 0), end(result));
  ASSERT_NE(std::find(begin(result), end(result), 1), end(result));
}


}