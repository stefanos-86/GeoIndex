#ifndef CUBEINDEX_H
#define CUBEINDEX_H

/** Different take on the problem. Divide the space in cubes, remember wich cube hosts wich point, then
 seek the "interesting" points in the cube or its neighbors in space.
 
 Could take advantage from a voxel library, but I want to avoid dependencies in the demo.
 */
#include <unordered_map>
#include <vector>

#include "Support.hpp"

namespace geoIndex {
  
  struct Cube {
    Cube();
    std::vector<PointIndex> pointsInside;
  };
  
  typedef int64_t CubicCoordinate;
  
  /* Group of cubes in the space. Cube coordinates are i, j, k not to mix up with points that use x, y, z. */
  class CubeCollection {
  public:
    CubeCollection();
    void insert(const CubicCoordinate i,
                const CubicCoordinate j,
                const CubicCoordinate k,
                const PointIndex index);
    
    const std::vector<PointIndex>& read(const CubicCoordinate i,
                                        const CubicCoordinate j,
                                        const CubicCoordinate k) const;
    
  private:
  /* Alternative: the usual 3D matrix. But with that (vector in vector in vector) I would have to know the size in advance.
     That would give direct access, this may work better if there are many empty cubes (that don't get created). */
    std::unordered_map<CubicCoordinate,  // i
                      std::unordered_map<CubicCoordinate, // j
                                        std::unordered_map<CubicCoordinate, Cube> > > cubes;  // k and content.
// TODO: may be worth to check the hashing. And also a map that index on (i, j, k) triplets or other i, j, k combinations.
  };
  
  class CubeIndex {
  public:
    CubeIndex(const double step);
    void addOnePoint(const Point& p, const PointIndex i);
    void closeTo(const Point& p, std::vector<PointIndex>& closePoints) const;
    
  private:
    CubeCollection index;
    const double step;  ///< Side of the cubes. Small step, high grid resolution, but more cubes needed (and more difficult to find neighbors).
    
    /** To convert from the x, y, z coordinates of points to the discreet coordinates of cubes. */
    CubicCoordinate spaceToCubic(const double coordinate) const;
  };
  
}

#endif