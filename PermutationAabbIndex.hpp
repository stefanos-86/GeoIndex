#ifndef GEOINDEX_PERMUTATIONAABB_INDEX
#define GEOINDEX_PERMUTATIONAABB_INDEX

#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <unordered_map>

#include "Common.hpp"

namespace geoIndex {
    
 
  /** Variation on the AABB index that uses a different structure to store the position of the points.
   *  It uses 3 arrays for the point coordinates, sorted by coordinates.
   *  Then it uses 3 more arrays to store the permutation that gives the point index for each of the other 
   *  arrays.
   * 
   * Inspired by the much, much more optimized data structure in "Compact Data Strucutres, a practical approach"
   * (by Gonzalo Navarro). See its section 5.4.1.
   * 
   * It's still rubbish, but that's likely to be my fault. It was worth an attempt, but it did not work as hoped.
   */
template <typename POINT>
class PermutationAabbIndex {
public:
    
    /** If you know how many points you are going to use, tell it to this constructor to 
    *  reserve memory. */
    PermutationAabbIndex(const size_t expectedCollectionSize = 0) {
        coordinatesX.reserve(expectedCollectionSize);
        coordinatesY.reserve(expectedCollectionSize);
        coordinatesZ.reserve(expectedCollectionSize);
        
        permuatationX.reserve(expectedCollectionSize);
        permuatationY.reserve(expectedCollectionSize);
        permuatationZ.reserve(expectedCollectionSize);
        
        #ifdef GEO_INDEX_SAFETY_CHECKS
            // There is nothing in the index, so it is sorted (you can't misplace... nothing). You can do lookups.
            readyForLookups = true;
        #endif
    }
    
    
    /** Adds a point to the index. Remember its name too. */
    void index(const POINT& p, const typename PointTraits<POINT>::index index){
        #ifdef GEO_INDEX_SAFETY_CHECKS
            // Adding points probably breaks the order. Use X indexes for the check, y and z are equal at this stage.
            readyForLookups = false;
            
            if (std::find(begin(indices), end(indices), index)
                    != end(indices))
                throw std::runtime_error("Point indexed twice");
        #endif
            
       coordinatesX.push_back(p.x);
       coordinatesY.push_back(p.y);
       coordinatesZ.push_back(p.z);
       
       indices.push_back(index);
       
       // Just make sure the permutation vectors grow as much as the coordinates vectors.
       // Values are to be calculated later on.
       permuatationX.push_back(0);
       permuatationY.push_back(0);
       permuatationZ.push_back(0);
    }
    
    
    /** "Close" the index and allows lookups.
     *   Basically, sort its entries. It is best done "once and forever" as testing proves it can be a significant bottleneck.
     *   If the user forgets to call it he will get garbage results.
     */
    void completed() {
        std::iota(std::begin(permuatationX), std::end(permuatationX), 0);
        std::copy(std::begin(permuatationX), std::end(permuatationX), std::begin(permuatationY));
        std::copy(std::begin(permuatationX), std::end(permuatationX), std::begin(permuatationZ));
                
        permutationIndex(coordinatesX, permuatationX);
        permutationIndex(coordinatesY, permuatationY);
        permutationIndex(coordinatesZ, permuatationZ);
        
        
        std::sort(std::begin(coordinatesX), std::end(coordinatesX));
        std::sort(std::begin(coordinatesY), std::end(coordinatesY));
        std::sort(std::begin(coordinatesZ), std::end(coordinatesZ));
        
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
        
        // Count if the point is present in all the 3 sets of candidates, assuming the sets are
        // sparse, much less than the whole amount of points.
        std::unordered_map<typename PointTraits<POINT>::index, uint8_t> hitsPerIndex;
        std::unordered_map<typename PointTraits<POINT>::index, POINT> candidatePoints;
            
        const auto extremesOnX = candidatesOnDimension(coordinatesX, d, p.x);
        const auto extremesOnY = candidatesOnDimension(coordinatesY, d, p.y);
        const auto extremesOnZ = candidatesOnDimension(coordinatesZ, d, p.z);
        
        // We know the points between the extremes on X may be good.
        // Now we fetch their index, then look into Y and Z by index.
        for (size_t point = extremesOnX.first; point < extremesOnX.second; ++point)
        {
            const auto indexOfPoint = indices[permuatationX[point]];
            hitsPerIndex[indexOfPoint] = 1;
            candidatePoints[indexOfPoint] = POINT{coordinatesX[point], 0, 0};
        }
        
        for (size_t point = extremesOnY.first; point < extremesOnY.second; ++point)
        {
            const auto indexOfPoint = indices[permuatationY[point]];
            if (hitsPerIndex.find(indexOfPoint) != std::end(hitsPerIndex))
            {
                hitsPerIndex[indexOfPoint] += 1;
                candidatePoints[indexOfPoint].y = coordinatesY[point];
            }
        }
        
        for (size_t point = extremesOnZ.first; point < extremesOnZ.second; ++point)
        {
            const auto indexOfPoint = indices[permuatationZ[point]];
            if (hitsPerIndex.find(indexOfPoint) != std::end(hitsPerIndex))
            {
                hitsPerIndex[indexOfPoint] += 1;
                candidatePoints[indexOfPoint].z = coordinatesZ[point];
            }
        }
        
        const typename PointTraits<POINT>::coordinate referenceSquareDistance = d * d; 
        #ifdef GEO_INDEX_SAFETY_CHECKS
            CheckOverflow(referenceSquareDistance);
        #endif
       
        output.clear();
        for (const auto& candidatePoint : candidatePoints)
        {
            const auto pointIndex = candidatePoint.first;
            if (hitsPerIndex[pointIndex] == 3) // Point found in all the 3 candidate sets.
            {
                const auto candidateSquareDistance = SquaredDistance(p, candidatePoint.second);
                if (candidateSquareDistance < referenceSquareDistance)
                    output.push_back({pointIndex, candidateSquareDistance});
            }
        }
        
        // Don't forget we have to give the closests point first.
        std::sort(std::begin(output), std::end(output), SortByGeometry<POINT>);
    }
    
private:
    std::vector<typename PointTraits<POINT>::coordinate> coordinatesX;
    std::vector<typename PointTraits<POINT>::coordinate> coordinatesY;
    std::vector<typename PointTraits<POINT>::coordinate> coordinatesZ;
    
    std::vector<size_t> permuatationX;
    std::vector<size_t> permuatationY;
    std::vector<size_t> permuatationZ;
    
    std::vector<typename PointTraits<POINT>::index> indices;
    
    #ifdef GEO_INDEX_SAFETY_CHECKS
        bool readyForLookups;
    #endif
  
        
    /** Shuffle the positions so that it gets permutated just like the coordinate vector would be
        if it is sorted. Acts on the indexes in place.
        
        Rip off of https://stackoverflow.com/questions/17554242/how-to-obtain-the-index-permutation-after-the-sorting
    */
    static void permutationIndex(const std::vector<typename PointTraits<POINT>::coordinate>& coordinates,
                                 std::vector<size_t>& positions)
    {
        std::sort(std::begin(positions), std::end(positions),
            [&](const size_t& a, const size_t& b) {
                return (coordinates[a] < coordinates[b]);
            });
    }
   

       
    /** Scan the given index and returns the position in the array of the "extreme" points that 
        mark the begin and "past end" of the desired points.*/
    std::pair<size_t, size_t>
    candidatesOnDimension(const std::vector<typename PointTraits<POINT>::coordinate>& indexForDimension,
                          const typename PointTraits<POINT>::coordinate searchDistance,
                          const typename PointTraits<POINT>::coordinate referenceCoordnate) const 
    {
        const typename PointTraits<POINT>::coordinate minAcceptedCoordinate = referenceCoordnate - searchDistance;
        const typename PointTraits<POINT>::coordinate maxAcceptedCoordinate = referenceCoordnate + searchDistance;

        const auto beginCandidates = std::lower_bound(std::begin(indexForDimension),
                                                      std::end(indexForDimension),
                                                      minAcceptedCoordinate);
        
        const auto endCandidates = std::lower_bound(beginCandidates,  // the bigger values must be after, skip some elements.
                                                    std::end(indexForDimension),
                                                    maxAcceptedCoordinate);
        
        const size_t positionFirstGoodPoint = std::distance(begin(indexForDimension), beginCandidates);
        const size_t positionPastLastPoint = std::distance(begin(indexForDimension), endCandidates);
        return std::make_pair(positionFirstGoodPoint, positionPastLastPoint);
        
    }    

    
};
  
  
}

#endif
