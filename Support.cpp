#include "Support.hpp"

#include <cmath>
#include <cerrno>
#include <cfenv>
#include <stdexcept>

namespace geoIndex {
  
static double SafeSqrt(const double value) {
  errno = 0;
  std::feclearexcept(FE_ALL_EXCEPT);
  
  double squareRoot = std::sqrt(value);
  
  if (errno != 0)
    throw std::runtime_error("Problem with the square root");
  
  return squareRoot;
}


double Distance(const Point& p1, const Point& p2) {
  const double xDistance = p1.x - p2.x;
  const double yDistance = p1.y - p2.y;
  const double zDistance = p1.z - p2.z;
  
  const double squaredDistance = 
    xDistance * xDistance +
    yDistance * yDistance +
    zDistance * zDistance;
    

  return SafeSqrt(squaredDistance);
}

}