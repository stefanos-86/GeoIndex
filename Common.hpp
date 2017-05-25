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
struct IndexAndDistance {
  typename POINT::index_t pointIndex;
  typename POINT::coordinate_t squaredDistance;
};

template <typename POINT>
inline bool operator< (const IndexAndDistance<POINT>& lhs, const IndexAndDistance<POINT>& rhs){ 
return lhs.squaredDistance < rhs.squaredDistance; 
}

/* Those are not really needed, but it is "polite" to give the whole set. */
template <typename POINT>
inline bool operator> (const IndexAndDistance<POINT>& lhs, const IndexAndDistance<POINT>& rhs){ return rhs < lhs; }

template <typename POINT>
inline bool operator<=(const IndexAndDistance<POINT>& lhs, const IndexAndDistance<POINT>& rhs){ return !(lhs > rhs); }

template <typename POINT>
inline bool operator>=(const IndexAndDistance<POINT>& lhs, const IndexAndDistance<POINT>& rhs){ return !(lhs < rhs); }

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