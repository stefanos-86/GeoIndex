#ifndef GEOINDEX_BASIC_GEOMETRY
#define GEOINDEX_BASIC_GEOMETRY

#include <cstring>
#include <type_traits>
#ifdef GEO_INDEX_SAFETY_CHECKS
  #include <stdexcept>
  #include <cmath>
#endif

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
 *   An integer type. */
typedef size_t PointIndex;

/** The type used to define geometric coordinates (the, x, y, z in space).
 *  It must be possible to do arithmetic with it.
 *  Float or double should do.*/
typedef double Coordinate;
  
/** Point in 3D space, as simple as it can be. 
 *  Must have x, y, z properties and have typenames for coordinate_t and index_t.
 *  Does not store the index to keep the structure as compact as possible.
 */
template<typename COORDINATE>
struct BasicPoint {
  COORDINATE x;
  COORDINATE y;
  COORDINATE z;
};

class NotAPoint{};

/** Specialize this to adapt to various kind of point classes, if the need be.
 *  It is up to the user to write the specialization. Should it be missing, there should be a nice compile-time error
 *  to remind the programmer to make one.
 */
template<typename POINT>
struct PointTraits {
    /* The compiler "elaborates" static_assert even before instantiating the template (and this seems to be
        legal (https://stackoverflow.com/questions/5246049/c11-static-assert-and-template-instantiation).
        But if we use is_same, the compiler can't pre-compute the value, even if it is always false.
        With that, it should stop the compilation only when this struct/template is actually instantiated. */
    static_assert(std::is_same<POINT, NotAPoint>::value, "You forgot to specialize PointTraits for your class.");
};


/** Default "ready to use" point and its traits. */
typedef BasicPoint<Coordinate> Point;

template <>
struct PointTraits<Point> {
  typedef Coordinate coordinate;
  typedef PointIndex index;
};



/** Another type of point that we may see often. */
typedef BasicPoint<float> FloatPoint;

template <>
struct PointTraits<FloatPoint> {
  typedef float coordinate;
  typedef PointIndex index;
};


/** Computes the square of the distance of two points. 
 *  Used internally by the rest of the library.
 * 
 *  Since it is enough to compare the relative distances of points there is no need to have
 *  the real distance. 
 */
template <typename POINT>
typename PointTraits<POINT>::coordinate SquaredDistance(const POINT& p1, const POINT& p2) {
  const typename PointTraits<POINT>::coordinate xDistance = p1.x - p2.x;
  const typename PointTraits<POINT>::coordinate yDistance = p1.y - p2.y;
  const typename PointTraits<POINT>::coordinate zDistance = p1.z - p2.z;
  
  const typename PointTraits<POINT>::coordinate squaredDistance = 
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