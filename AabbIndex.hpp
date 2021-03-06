#ifndef GEOINDEX_AABB_INDEX
#define GEOINDEX_AABB_INDEX

#include <vector>
#include <algorithm> // Or #include <parallel/algorithm>.
#include <iterator>

#include "Common.hpp"

namespace geoIndex {
    
 
  /** This index uses the "Axis Aligned Bounding Box" (AABB) trick to quickly find points close to the 
   *  given reference. Imagine a box, with sides parallel to the axes. That is the AABB.
   *  Its position makes it very easy to discover points inside: just ensure that each coordinate
   *  it between the min/max coordinates inside the box. We only have to compute the "top" and "bottom" corners.
   *  To go even faster, we keep the point coordinates sorted, so in a single pass (on each x, y, z) we can find 
   *  all the indexes in the box limit.
   * 
   *  At each query it builds the AABB centered on the reference point and work on what is inside.
   * 
   *  The user must call complete() between modifications and lookups. But this is a costly operation.
   * 
   */
template <typename POINT>
class AabbIndex {
public:
    
    /** If you know how many points you are going to use, tell it to this constructor to 
    *  reserve memory. */
    AabbIndex(const size_t expectedCollectionSize = 0) {
        indexX.reserve(expectedCollectionSize);
        indexY.reserve(expectedCollectionSize);
        indexZ.reserve(expectedCollectionSize);
        
        #ifdef GEO_INDEX_SAFETY_CHECKS
            // There is nothing in the index, so it is sorted (you can't misplace... nothing). You can do lookups.
            readyForLookups = true;
        #endif
    }
    
    
    /** Adds a point to the index. Remember its name too. */
    void index(const POINT& p, const typename PointTraits<POINT>::index index){
        #ifdef GEO_INDEX_SAFETY_CHECKS
            // Adding points probably breaks the order.
            readyForLookups = false;
            
            if (std::find_if(begin(indexX),
                             end(indexX),
                             [index](const IndexAndCoordinate<POINT>& indexEntry) {
                                return indexEntry.pointIndex == index; }
                ) != end(indexX))
                throw std::runtime_error("Point indexed twice");
        #endif
            
       indexX.push_back({index, p.x});
       indexY.push_back({index, p.y});
       indexZ.push_back({index, p.z});
    }
    
    
    /** "Close" the index and allows lookups.
     *   Basically, sort its entries. It is best done "once and forever" as testing proves it can be a significant bottleneck.
     *   If the user forgets to call it he will get garbage results.
     */
    void completed() {
        std::sort(std::begin(indexX), std::end(indexX), SortByGeometry<POINT>);
        std::sort(std::begin(indexY), std::end(indexY), SortByGeometry<POINT>);
        std::sort(std::begin(indexZ), std::end(indexZ), SortByGeometry<POINT>);
        
        /* The latest STL (C++17) has a parallel sort that could be useful...
         * In the meanwhile, there is this nice thing from GNU.
            __gnu_parallel::sort(indexX.begin(), indexX.end());
            __gnu_parallel::sort(indexY.begin(), indexY.end());
            __gnu_parallel::sort(indexZ.begin(), indexZ.end());
            ...but depends on openMP (libgomp) - for the moment I don't do it, to keep deps simple.
            The speedup we can get here should be tested.
        */
        #ifdef GEO_INDEX_SAFETY_CHECKS
            readyForLookups = true;
        #endif
        
    }
    
    /** Finds the points that are within distance d from p. Cleans the output vector before filling it.
    *  Returns the points sorted in distance order from p (to simplify computing the k-nearest-neighbor).
    *  The returned structure also gives the squared distance. The client can do a sqrt and use it for its computations.
    * 
    *  Uses the AABB trick described on top of the class to speed up the search for close points.
    * 
    *  Returns only points strictly within the AABB.
    */
    void pointsWithinDistance(const POINT& p, 
                              const typename PointTraits<POINT>::coordinate d,
                              std::vector<IndexAndSquaredDistance<POINT> >& output) const 
    {
        #ifdef GEO_INDEX_SAFETY_CHECKS
            CheckMeaningfulDistance(d);
            if (! readyForLookups)
                throw std::runtime_error("Index not ready. Did you call completed() after the last call to index(...)?");
        #endif
        

        std::vector<IndexAndCoordinate<POINT> > candidatesX;
        std::vector<IndexAndCoordinate<POINT> > candidatesY;
        std::vector<IndexAndCoordinate<POINT> > candidatesZ;
        
        candidatesOnDimension(indexX, d, p.x, candidatesX);
        candidatesOnDimension(indexY, d, p.y, candidatesY);
        candidatesOnDimension(indexZ, d, p.z, candidatesZ);
        
        std::vector<IndexAndCoordinate<POINT> > insideAabbXY;
        std::set_intersection(std::begin(candidatesX),
                              std::end(candidatesX),
                              std::begin(candidatesY),
                              std::end(candidatesY),
                              std::back_inserter(insideAabbXY),
                              SortByPointIndex<POINT>
                             );
        
        std::vector<IndexAndCoordinate<POINT> > insideAabb;
        std::set_intersection(std::begin(insideAabbXY),
                              std::end(insideAabbXY),
                              std::begin(candidatesZ),
                              std::end(candidatesZ),
                              std::back_inserter(insideAabb),
                              SortByPointIndex<POINT>
                             );
        
        const typename PointTraits<POINT>::coordinate referenceSquareDistance = d * d;
        
        #ifdef GEO_INDEX_SAFETY_CHECKS
            CheckOverflow(referenceSquareDistance);
        #endif
            
        output.clear();
        for (const auto& candidate : insideAabb) {
            const auto candidateIndex = candidate.pointIndex;
            const typename PointTraits<POINT>::coordinate candidateX = findCoordinateOf(candidateIndex, candidatesX);
            const typename PointTraits<POINT>::coordinate candidateY = findCoordinateOf(candidateIndex, candidatesY);
            const typename PointTraits<POINT>::coordinate candidateZ = findCoordinateOf(candidateIndex, candidatesZ);
            
            const auto candidateSquareDistance = SquaredDistance(p, POINT{candidateX, candidateY, candidateZ});
            
            if (candidateSquareDistance < referenceSquareDistance)
                output.push_back({candidateIndex, candidateSquareDistance});
        }
        
        // Don't forget we have to give the closests point first.
        std::sort(std::begin(output), std::end(output), SortByGeometry<POINT>);
    }
    
private:
    std::vector<IndexAndCoordinate<POINT> > indexX;
    std::vector<IndexAndCoordinate<POINT> > indexY;
    std::vector<IndexAndCoordinate<POINT> > indexZ;
    
    #ifdef GEO_INDEX_SAFETY_CHECKS
        bool readyForLookups;
    #endif
  
   
    
    static bool CompareEntryWithCoordinate(const IndexAndCoordinate<POINT>& indexEntry,
                                           const typename PointTraits<POINT>::coordinate searchedValue) {
        return indexEntry.geometricValue < searchedValue;
    }
    
    static bool CompareEntryWithIndex(const IndexAndCoordinate<POINT>& indexEntry,
                                      const typename PointTraits<POINT>::index searchedValue) {
        return indexEntry.pointIndex < searchedValue;
    }
       
    /** Scan the given index and returns the indices of the "interesting" points.*/
    void candidatesOnDimension(const std::vector<IndexAndCoordinate<POINT> >& indexForDimension,
                               const typename PointTraits<POINT>::coordinate searchDistance,
                               const typename PointTraits<POINT>::coordinate referenceCoordnate,
                               std::vector<IndexAndCoordinate<POINT> >& candidates) const 
    {
        const typename PointTraits<POINT>::coordinate minAcceptedCoordinate = referenceCoordnate - searchDistance;
        const typename PointTraits<POINT>::coordinate maxAcceptedCoordinate = referenceCoordnate + searchDistance;

        const auto beginCandidates = std::lower_bound(std::begin(indexForDimension),
                                                      std::end(indexForDimension),
                                                      minAcceptedCoordinate,
                                                      CompareEntryWithCoordinate);
        
        const auto endCandidates = std::lower_bound(beginCandidates,  // the bigger values must be after, skip some elements.
                                                    std::end(indexForDimension),
                                                    maxAcceptedCoordinate,
                                                    CompareEntryWithCoordinate);
        
        candidates.clear();
        candidates.reserve(std::distance(beginCandidates, endCandidates));
        for (auto i = beginCandidates; i != endCandidates; ++i)
            candidates.push_back(*i);
        
        // TODO: would it be faster with sets? THIS IS A MAJOR BOTTLENECK... in callgrind only?
       //std::sort(std::begin(candidates), std::end(candidates), SortByPointIndex<POINT>);
        
        std::sort(std::begin(candidates), std::end(candidates), [](const IndexAndGeometry<POINT>& lhs,
                             const IndexAndGeometry<POINT>& rhs)
        {return lhs.pointIndex < rhs.pointIndex;});
        
    }    
    
    /** This index does not store the full collection of points (but maybe it should, or should have a reference to it,
      * to avoid this kind of issues - or maybe we should move the distance test outside the indexes). 
      * Assume that groupOfCandidates is sorted by point index and point indices do not repeat. */
    typename PointTraits<POINT>::coordinate findCoordinateOf(const typename PointTraits<POINT>::index pointIndex,
                                                             const std::vector<IndexAndCoordinate<POINT> >& groupOfCandidates) const
    {
        const auto indexEntry = std::lower_bound(std::begin(groupOfCandidates),
                                                 std::end(groupOfCandidates),
                                                 pointIndex,
                                                 CompareEntryWithIndex);
        
        return indexEntry->geometricValue;
        
    }
    
};
  
  
}

#endif
