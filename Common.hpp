#ifndef GEOINDEX_COMMON
#define GEOINDEX_COMMON

#include <vector>

#ifdef GEO_INDEX_SAFETY_CHECKS
    #include <stdexcept>
    #include <cmath>
    #include<limits>
#endif

namespace geoIndex {

/** This file collects elements that are common to all the algorithms. */

/** This struct is useful for sorting operations on points or coordinates or distances.... 
 *  It has no actual point inside to keep it as small as it can get. 
 *  The content is not constant to allow auto-generation of constructors, operators... (try to put const there!).
 */
template <typename POINT>
struct IndexAndGeometry {
  typename POINT::index_t pointIndex;
  typename POINT::coordinate_t geometricValue;
};

/** Helpers to sort and search. */
template <typename POINT>
static bool SortByPointIndex(const IndexAndGeometry<POINT>& lhs,
                             const IndexAndGeometry<POINT>& rhs)
{
    return lhs.pointIndex < rhs.pointIndex;
}

template <typename POINT>
static bool SortByGeometry(const IndexAndGeometry<POINT>& lhs,
                           const IndexAndGeometry<POINT>& rhs)
{
    return lhs.geometricValue < rhs.geometricValue;
}  


/** Alias from when we store the distance for a point to a reference. */
template <typename POINT>
using IndexAndSquaredDistance = IndexAndGeometry<POINT>;

/** Alias to store coordinates. */
template <typename POINT>
using IndexAndCoordinate = IndexAndGeometry<POINT>;

//TODO: test the stuff here directly.
#ifdef GEO_INDEX_SAFETY_CHECKS

template <typename POINT_COORDINATE>
void CheckOverflow(const POINT_COORDINATE value) {
    if (std::isinf(value))
          throw std::runtime_error("Overflow");
}

template <typename DISTANCE_TYPE>
void CheckMeaningfulCullingDistance(const DISTANCE_TYPE d) {
    if (d <= 0)
        throw std::runtime_error("Negative distance");
    if (std::isnan(d))
        throw std::runtime_error("Invalid distance");
}  

/** Throws if a + b would go over the maximum. 
  * We are solidly in "best quick and dirty effort" territory. This probably has holes,
  * but may still help to smoke out bugs. */
template <typename T>
void StopSumOverflow(const T a, const T b) {
    if (std::numeric_limits<T>::max() - a < b)
        throw std::runtime_error("Sum about to overflow.");
    
}

/** Throws if a - b would go below the minimum.
  * We are solidly in "best quick and dirty effort" territory. This probably has holes,
  * but may still help to smoke out bugs. */
template <typename T>
void StopDifferenceUnderflow(const T a, const T b) {
    if (a < std::numeric_limits<T>::min() + b)
        throw std::runtime_error("Difference about to underflow.");
    
}
#endif


}




#endif