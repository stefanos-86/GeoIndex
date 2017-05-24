#include "CubeIndex.hpp" 

namespace geoIndex {

Cube::Cube() {
  pointsInside.reserve(50);  // Tweak to balance memory usage and init time.
}


CubeCollection::CubeCollection() {
   cubes.reserve(10);  // May help to reduce the mallocs.
}


  
void CubeCollection::insert(const CubicCoordinate i,
                            const CubicCoordinate j,
                            const CubicCoordinate k,
                            const PointIndex index) {
  auto& insertHere = cubes[i][j][k];
  insertHere.pointsInside.push_back(index);
}

const std::vector< PointIndex >& CubeCollection::read(const CubicCoordinate i,
                                                      const CubicCoordinate j,
                                                      const CubicCoordinate k) const {

  static const Cube empty;  // Void return value.
  
  const auto sliceAtI = cubes.find(i);
  if (sliceAtI == cubes.end())
    return empty.pointsInside;
  
  const auto rowAtIJ = sliceAtI->second.find(j);
  if (rowAtIJ == sliceAtI->second.end())
    return empty.pointsInside;
  
  const auto cubeAtIJK = rowAtIJ->second.find(k);
  if (cubeAtIJK == rowAtIJ->second.end())
    return empty.pointsInside;
  
  return cubeAtIJK->second.pointsInside;
}



CubeIndex::CubeIndex(const double step) :
  step(step) { 
}


void CubeIndex::addOnePoint(const Point& p, const PointIndex i) {
  index.insert(spaceToCubic(p.x),
               spaceToCubic(p.y),
               spaceToCubic(p.z),
               i);
}

void CubeIndex::closeTo(const Point& p, std::vector< PointIndex >& closePoints) const {
  closePoints.clear();
  const CubicCoordinate iReference = spaceToCubic(p.x);
  const CubicCoordinate jReference = spaceToCubic(p.y);
  const CubicCoordinate kReference = spaceToCubic(p.z);
  
  // Scan all the cubes around the one that contains the reference point.
  for (CubicCoordinate i = iReference - 1; i <= iReference + 1; i++)
    for (CubicCoordinate j = jReference - 1; j <= jReference + 1; j++)
      for (CubicCoordinate k = kReference - 1; k <= kReference + 1; k++) {
          const auto& foundPoints = index.read(i, j, k);
          std::copy(foundPoints.begin(), foundPoints.end(), std::back_inserter(closePoints));
      }

}

CubicCoordinate CubeIndex::spaceToCubic(const double coordinate) const {
  // Finds the cube and truncates decimal in single to.
  // ...what will be the effect of math imprecision? Also: should ensure no overflow, but here I don't care.
  return static_cast<CubicCoordinate>(coordinate / step);
}



}