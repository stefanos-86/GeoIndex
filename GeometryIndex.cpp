#include "GeometryIndex.hpp"

#include <algorithm>

namespace geoIndex {
  
void GeometryIndex::index(const Point& p, const PointIndex index) {
  indexX.push_back({p.x, index});
  indexY.push_back({p.y, index});
  indexZ.push_back({p.z, index});
}

void GeometryIndex::prepareForLookups() {
  std::sort(begin(indexX), end(indexX));  // The latest STL has a parallel sort that could be useful...
  std::sort(begin(indexY), end(indexY));
  std::sort(begin(indexZ), end(indexZ));
}


// I had lots of troubles with the iterators - this may not be the right way.
static void CopyOnlyIndexes(size_t begin,
                            const size_t end,
                            const std::vector<IndexEntry>& source,
                            std::vector<PointIndex>& result) {
  result.reserve(end - begin);
  while (begin < end){
    result.push_back(source[begin].index);
    ++begin;
  }
    
}

static bool CompareEntryWithCoordinate(const IndexEntry& lhsEntry, const double rhsCoordinate) {
  return lhsEntry.coordinate < rhsCoordinate;
}

// Bad implementation: has triplicate code. Should have a generic lookup function to be called for x, y, z.
// Then only to the intersection of results.
void GeometryIndex::withinAabb(const Point& p, const double distance, std::vector< PointIndex >& closePoints) const {
  const size_t beginCandidatesX = std::lower_bound(begin(indexX), end(indexX), p.x - distance, CompareEntryWithCoordinate) - begin(indexX);
  const size_t beginCandidatesY = std::lower_bound(begin(indexY), end(indexY), p.y - distance, CompareEntryWithCoordinate) - begin(indexY);
  const size_t beginCandidatesZ = std::lower_bound(begin(indexZ), end(indexZ), p.z - distance, CompareEntryWithCoordinate) - begin(indexZ);
  
  const size_t endCandidatesX = std::lower_bound(begin(indexX), end(indexX), p.x + distance, CompareEntryWithCoordinate) - begin(indexX);
  const size_t endCandidatesY = std::lower_bound(begin(indexY), end(indexY), p.y + distance, CompareEntryWithCoordinate) - begin(indexY);
  const size_t endCandidatesZ = std::lower_bound(begin(indexZ), end(indexZ), p.z + distance, CompareEntryWithCoordinate) - begin(indexZ);
  
  // The points in the bounding box are those that appears in all 3 sets.
  // I must copy the point indexes because set_intersection expects sorted sets.
  // This may become a bottleneck, but I must not rearrange the coordinate indexes (so they can be reused).
  // This must be profiled (...would it work better with sets? Can we avoid the copy?) if it must be made faster.
  std::vector<PointIndex> candidatesX, candidatesY, candidatesZ;
  CopyOnlyIndexes(beginCandidatesX, endCandidatesX, indexX, candidatesX);
  CopyOnlyIndexes(beginCandidatesY, endCandidatesY, indexY, candidatesY);
  CopyOnlyIndexes(beginCandidatesZ, endCandidatesZ, indexZ, candidatesZ);
  std::sort(begin(candidatesX), end(candidatesX));
  std::sort(begin(candidatesY), end(candidatesY));
  std::sort(begin(candidatesZ), end(candidatesZ));
  
  closePoints.clear();
  std::vector<PointIndex> insideAabbXY;
  // reserve?
  std::set_intersection(begin(candidatesX), end(candidatesX), begin(candidatesY), end(candidatesY), std::back_inserter(insideAabbXY));
  std::set_intersection(begin(insideAabbXY), end(insideAabbXY), begin(candidatesZ), end(candidatesZ), std::back_inserter(closePoints));
}



  
}