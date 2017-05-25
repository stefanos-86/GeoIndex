#ifndef GEOINDEX_COMMON
#define GEOINDEX_COMMON

/** This file collects elements that are common to all the algorithms (other than the basic geometry). */

/** This struct is useful for sorting operations on points. 
 *  It has no actual point inside to keep it as small as it can get. 
 *  The content is not constant to allow auto-generation of constructors, operators... (try to put const there!).
 */
template <typename POINT>
struct IndexByDistance {
  typename POINT::index_t pointIndex;
  typename POINT::coordinate_t squaredDistance;
};

template <typename POINT>
inline bool operator< (const IndexByDistance<POINT>& lhs, const IndexByDistance<POINT>& rhs){ 
return lhs.squaredDistance < rhs.squaredDistance; 
}

/* Those are not really needed, but it is "polite" to give the whole set. */
template <typename POINT>
inline bool operator> (const IndexByDistance<POINT>& lhs, const IndexByDistance<POINT>& rhs){ return rhs < lhs; }

template <typename POINT>
inline bool operator<=(const IndexByDistance<POINT>& lhs, const IndexByDistance<POINT>& rhs){ return !(lhs > rhs); }

template <typename POINT>
inline bool operator>=(const IndexByDistance<POINT>& lhs, const IndexByDistance<POINT>& rhs){ return !(lhs < rhs); }

#endif