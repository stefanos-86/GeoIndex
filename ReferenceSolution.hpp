#ifndef REFERENCESOLUTION_H
#define REFERENCESOLUTION_H

#include <utility>
#include <vector>

#include "Support.hpp"

namespace geoIndex {
  
  /** Represents the original method to solve the problem.
      Provides a baseline for performance comparisons. */
  
  struct Point;
  
  std::pair<PointIndex, PointIndex> TwoClosestPoints(const Point& basePoint, const std::vector<Point>& pointCollection);
  
};

#endif