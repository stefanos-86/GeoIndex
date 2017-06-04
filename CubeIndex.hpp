#ifndef GEOINDEX_CUBE_INDEX
#define GEOINDEX_CUBE_INDEX

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

#include "Common.hpp"
#include "BasicGeometry.hpp"

#ifdef GEO_INDEX_SAFETY_CHECKS
    #include <limits>
#endif

namespace geoIndex {
  
    /** Space partition unit. Notice that we only know the indexes at this level. */
    template <typename POINT>
    struct Cube {
        std::vector<typename POINT::index_t> indexOfPointsInside;
    };
  
    typedef int64_t CubicCoordinate;
  
    /* Group of cubes in the space. Cube coordinates are i, j, k so we don't mix up with points that use x, y, z. */
    template <typename POINT>
    class CubeCollection {
    public:
        void insert(const CubicCoordinate i,
                    const CubicCoordinate j,
                    const CubicCoordinate k,
                    const typename POINT::index_t index
                   ) {
            auto& cubeForInsertion = cubes[i][j][k];
            cubeForInsertion.indexOfPointsInside.push_back(index);
        }

        const std::vector<typename POINT::index_t>& read(const CubicCoordinate i,
                                                         const CubicCoordinate j,
                                                         const CubicCoordinate k) const
       {
             static const Cube<POINT> empty;  // Void return value.
  
            const auto sliceAtI = cubes.find(i);
            if (sliceAtI == cubes.end())
                return empty.indexOfPointsInside;
            
            const auto rowAtIJ = sliceAtI->second.find(j);
            if (rowAtIJ == sliceAtI->second.end())
                return empty.indexOfPointsInside;
            
            const auto cubeAtIJK = rowAtIJ->second.find(k);
            if (cubeAtIJK == rowAtIJ->second.end())
                return empty.indexOfPointsInside;
            
            return cubeAtIJK->second.indexOfPointsInside;
       }
       
    private:
    /* Alternative: the usual 3D matrix. But with that (vector in vector in vector) I would have to know the size in advance.
     That would give direct access, this may work better if there are many empty cubes (that don't get created). */
    std::unordered_map<CubicCoordinate,  // i
                      std::unordered_map<CubicCoordinate, // j
                                        std::unordered_map<CubicCoordinate, Cube<POINT> > > > cubes;  // k and content.
// TODO: may be worth to check the hashing. And also a map that index on (i, j, k) triplets or other i, j, k combinations.
    };
    

/** Different take on the problem. Divide the space in cubes, remember wich cube hosts wich point, then
 *  seek the "interesting" points in the cube or its neighbors in space.
 * 
 *  Could take advantage from a voxel library, but I want to avoid dependencies.
 */
template <typename POINT>
class CubeIndex {
public:
    /** Creates an index that divides the space in cubes of the given side size. */
    CubeIndex(typename Point::coordinate_t cubeSide) :
        gridStep(cubeSide)
    {}

    /** Adds a point to the index. Remember its name too. */
    void index(const POINT& p, const typename POINT::index_t index){
        #ifdef GEO_INDEX_SAFETY_CHECKS
            if (points.find(index) != end(points))
                throw std::runtime_error("NoIndex::index Point indexed twice");
        #endif
        
        cubes.insert(spaceToCubic(p.x),
                     spaceToCubic(p.y),
                     spaceToCubic(p.z),
                     index);
        points[index] = p;
        
    }
    
    /** Does nothing. This index does not require any "pre-processing" and can be used after indexing "new" points. */
    void completed() {};
    
    void pointsWithinDistance(const POINT& p, 
                              const typename POINT::coordinate_t d,
                              std::vector<IndexAndSquaredDistance<POINT> >& output) const {
        #ifdef GEO_INDEX_SAFETY_CHECKS
            CheckMeaningfulCullingDistance(d);
        #endif
            
        const CubicCoordinate iReference = spaceToCubic(p.x);
        const CubicCoordinate jReference = spaceToCubic(p.y);
        const CubicCoordinate kReference = spaceToCubic(p.z);
        
        /* We must take the points in all the cubes that are closer than d to the reference.
        * Then we take cubes that are d on both sides.
        * The +1 guarantees that we "comfortably exceed" the distance, to compensate for truncated decimals */
        const CubicCoordinate dAsNumberOfCubes = static_cast<CubicCoordinate>(d / gridStep);
        const CubicCoordinate scanDistance = dAsNumberOfCubes + 1;
    
        // Scan all the cubes around the one that contains the reference point.
        std::vector<typename POINT::index_t> indicesOfCandidates;
        // TODO: under/overflow if cubic coordinates
        for (CubicCoordinate i = iReference - scanDistance; i <= iReference + scanDistance; i++)
            for (CubicCoordinate j = jReference - scanDistance; j <= jReference + scanDistance; j++)
                for (CubicCoordinate k = kReference - scanDistance; k <= kReference + scanDistance; k++) {
                    const auto& foundPoints = cubes.read(i, j, k);
                    std::copy(foundPoints.begin(), foundPoints.end(), std::back_inserter(indicesOfCandidates));
                }
                
        const auto distanceLimit = d * d;
                                                                   
        #ifdef GEO_INDEX_SAFETY_CHECKS
            CheckOverflow(distanceLimit);
        #endif
        
        // Now take only the points really inside d and sort the output.
        output.clear();
        output.reserve(indicesOfCandidates.size()); //May reserve more than necessary.
        for (const auto candidateIndex : indicesOfCandidates) {
            const POINT& candidate = points.find(candidateIndex)->second;
            const auto squaredDistance = SquaredDistance(p, candidate);

            
            if (squaredDistance < distanceLimit)
                output.push_back({candidateIndex, squaredDistance});
        }
        
        std::sort(std::begin(output), std::end(output), SortByGeometry<POINT>);
    }
                            
private:
    const typename POINT::coordinate_t gridStep;
    
    CubeCollection<POINT> cubes;
    std::unordered_map<typename POINT::index_t, POINT> points;  ///< Must keep track of the points for geometry computations.

    
    /** To convert from the x, y, z coordinates of points to the discreet coordinates of cubes. 
     *  The cubes divide the space in a uniform 3D grid, so finding the relevant cube is easy. Decimals are truncated
     *  (imagine the cubes aligned on integer coordinates in the grid reference system). 
     *
     *  Assume implicitely that coordinates are expressed in some "big" type, like double, that may have
     *  bigger values that the integer used for CubicCoordinate. */
    CubicCoordinate spaceToCubic(const typename POINT::coordinate_t coordinate) const 
    {
        // Finds the cube and truncates decimal in single to.
        const typename POINT::coordinate_t beforeTruncation = coordinate / gridStep;
        #ifdef GEO_INDEX_SAFETY_CHECKS
            if (beforeTruncation > std::numeric_limits<CubicCoordinate>::max())
                throw std::runtime_error("Cubic coordinate overflow");
        #endif
        return static_cast<CubicCoordinate>(beforeTruncation);
    }
};

}
#endif