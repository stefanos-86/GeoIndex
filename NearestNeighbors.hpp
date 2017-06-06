#ifndef GEOINDEX_NEAREST_NEIGHBORs
#define GEOINDEX_NEAREST_NEIGHBORs

#include <vector>
#include <type_traits>

#include "Common.hpp"

#ifdef GEO_INDEX_SAFETY_CHECKS
    #include <stdexcept>
#endif

namespace geoIndex {
    
/** Utility function to build the all the index in the same way.
 *  Automatically assigns the indices. This may not be what the user wants.
 *  But in this case, he will have to traverse the specific point collection by himself...
 * 
 *  Assumes that PointTraits<POINT>::index is an unsigned integral type.
 */
template <typename POINT, typename GEOMETRY_INDEX>
void BuildIndex(
    const std::vector<POINT> knownPoints,
    GEOMETRY_INDEX& resultingIndex
) {
   static_assert(std::is_unsigned<typename PointTraits<POINT>::index>::value,
                 "BuildIndex can only deal with unsigned integral types as indexes.");
    
    for (typename PointTraits<POINT>::index i = 0; i < knownPoints.size(); ++i)
        resultingIndex.index(knownPoints[i], i);
    
    resultingIndex.completed();
}

/** "Top level" algorithm of the library that solves the k-nearest-neighbor problem. 
 *   It takes the collection of points and the "speed-up-lookups" index that must have been previously built.
 *   Any point outside the culling distance won't be considered as a close neighbor, for speed.
 * 
 *   Must be given a reference to the full point collection, as the index may not store all the data.
 * 
 *   Returns the up to K closest points to the reference withing the culling distance.
 *   May return less than K if not enough points are within the culling distance from the reference.
 *   It is up to the user to remedy (e. g. to try a bigger distance).
 * 
 *   Notice that it also returns the (squared) distance from the points. I assume the user wants to do math with it
 *   (...because the only existing user told me so).
 */
template <typename POINT, typename GEOMETRY_INDEX>
void KNearestNeighbor(
    const GEOMETRY_INDEX& geometryIndex,
    const typename PointTraits<POINT>::coordinate cullingDistance,
    const POINT& referencePoint,
    const size_t k,
    typename std::vector<IndexAndSquaredDistance<POINT> >& output
    ) {
    
    #ifdef GEO_INDEX_SAFETY_CHECKS
        if (k == 0)
            throw std::runtime_error("KNearestNeighbor K can't be 0");
        
        if (cullingDistance <= 0)
            throw std::runtime_error("KNearestNeighbor Non-positive culling distance.");
    #endif
    
    output.clear();
    output.reserve(k); // We want k points, we find at least this many.
    geometryIndex.pointsWithinDistance(referencePoint, cullingDistance, output);

    if (output.size() > k)
        output.resize(k);
}


}

#endif