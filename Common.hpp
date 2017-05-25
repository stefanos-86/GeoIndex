#ifndef GEOINDEX_COMMON
#define GEOINDEX_COMMON

#include <vector>

#ifdef GEO_INDEX_SAFETY_CHECKS
  #include <stdexcept>
  #include <cmath>
#endif

namespace geoIndex {

/** This file collects elements that are common to all the algorithms. */

/** This struct is useful for sorting operations on points. 
 *  It has no actual point inside to keep it as small as it can get. 
 *  The content is not constant to allow auto-generation of constructors, operators... (try to put const there!).
 */
template <typename POINT>
struct IndexAndGeometry {
  typename POINT::index_t pointIndex;
  typename POINT::coordinate_t geometricValue;
};

// TODO: replace with sort by geometry or sort by index. Find them in the AABB implementation.
template <typename POINT>
inline bool operator< (const IndexAndGeometry<POINT>& lhs, const IndexAndGeometry<POINT>& rhs){ 
return lhs.geometricValue < rhs.geometricValue; 
}

/* Those are not really needed, but it is "polite" to give the whole set. */
template <typename POINT>
inline bool operator> (const IndexAndGeometry<POINT>& lhs, const IndexAndGeometry<POINT>& rhs){ return rhs < lhs; }

template <typename POINT>
inline bool operator<=(const IndexAndGeometry<POINT>& lhs, const IndexAndGeometry<POINT>& rhs){ return !(lhs > rhs); }

template <typename POINT>
inline bool operator>=(const IndexAndGeometry<POINT>& lhs, const IndexAndGeometry<POINT>& rhs){ return !(lhs < rhs); }


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
#endif


}




#endif