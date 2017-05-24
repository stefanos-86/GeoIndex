#ifndef GEOINDEX_BASIC_GEOMETRY
#define GEOINDEX_BASIC_GEOMETRY

#include <cstring>
#include <stdexcept>
#include <cmath>

/** Basic types and elements that we need to make the algorithms and indexes run.
 * Most probably the client already has his/her own. Otherwise, he can use those and run
 * without dependencies.
 * 
 * The "down to Earth" implementations we have here may not be the most efficient in all cases.
 * For example, smaller types may lead to less memory usage. Up to the user to check this.
 */

namespace geoIndex {

/** Type used to "label" a point in space. It is its name.
 *   Must be a value type that can be used as a key for lookups and a std::vector position.
 *  An integrap type. */
typedef size_t PointIndex;

/** The type used to define geometric coordinates (the, x, y, z in space).
 *  It must be possible to do arithmetic with it.
 *  Float or double should do.*/
typedef double Coordinate;
  
/** Point in 3D space, as simple as it can be. 
 *  Must have x, y, z properties. */
template<typename COORDINATE>
struct GenericPoint {
  typedef COORDINATE coordinate_t;
  COORDINATE x;
  COORDINATE y;
  COORDINATE z;
};

/** Default "ready to use" point. */
typedef GenericPoint<Coordinate> Point;

/** Another type of point that we may see often. */
typedef GenericPoint<float> FloatPoint;


/** Computes the square of the distance of two points. 
 *  Used internally by the rest of the library.
 * 
 *  Since it is enough to compare the relative distances of points there is no need to have
 *  the real distance. 
 */
template <typename POINT>
typename POINT::coordinate_t SquaredDistance(const POINT& p1, const POINT& p2) {
  const typename POINT::coordinate_t xDistance = p1.x - p2.x;
  const typename POINT::coordinate_t yDistance = p1.y - p2.y;
  const typename POINT::coordinate_t zDistance = p1.z - p2.z;
  
  const typename POINT::coordinate_t squaredDistance = 
    xDistance * xDistance +
    yDistance * yDistance +
    zDistance * zDistance;
   
#ifdef GEO_INDEX_SAFETY_CHECKS
    if (std::isinf(squaredDistance))
      throw std::runtime_error("Overflow");
#endif
  
  
  return squaredDistance;
}

}

#endif