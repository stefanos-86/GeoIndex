#include "ReferenceSolution.hpp"

#include <stdexcept>
#include <utility>
#include <cfloat>

#include "Support.hpp"

namespace geoIndex {

std::pair<PointIndex, PointIndex> TwoClosestPoints(const Point& basePoint, const std::vector<Point>& pointCollection) {
  if (pointCollection.size() < 2)
    throw std::runtime_error("Not enough points.");
  
  
  const double initialDistance1 = Distance(basePoint, pointCollection[0]);
  const double initialDistance2 = Distance(basePoint, pointCollection[1]);
  
  // Assume 1 closest than 2...
  PointIndex closest1 = 0;
  PointIndex closest2 = 1;
  double distance1 = initialDistance1;
  double distance2 = initialDistance2;
  
  // ...swap them if not the case.
  if (distance1 > distance2) {
    std::swap(closest1, closest2);
    std::swap(distance1, distance2);
  }
  
  
  for (PointIndex i = 2; i < pointCollection.size(); ++i) {  // Start from 2: the points at 0 and 1 have been tested already.
    const double candidateDistance = Distance(basePoint, pointCollection[i]);
    
    if (candidateDistance < distance1) {  // Better than the best?
        distance1 = candidateDistance;
        closest1 = i;
    } else if (candidateDistance < distance1) {  // No? Mabybe better than the 2nd best?
        distance2 = candidateDistance;
        closest2 = i;
    }
   
  }
  
  return std::make_pair(closest1, closest2);
}

}