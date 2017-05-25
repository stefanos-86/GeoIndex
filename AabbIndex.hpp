#ifndef GEOMETRYINDEX_H
#define GEOMETRYINDEX_H

#include <vector>
#include <cstring>

#include "Support.hpp"

namespace geoIndex {
  
  /** Allow to sort by coordinate, but remember the index matching it! 
   I stole the operator overloads directly from http://en.cppreference.com/w/cpp/language/operators.
   */
  struct IndexEntry {
    double coordinate;
    PointIndex index;
  };
  
    
  inline bool operator< (const IndexEntry& lhs, const IndexEntry& rhs){ return lhs.coordinate < rhs.coordinate; }
  
  /* Those are not really needed, but it is "polite" to give the whole set. */
  inline bool operator> (const IndexEntry& lhs, const IndexEntry& rhs){ return rhs < lhs; }
  inline bool operator<=(const IndexEntry& lhs, const IndexEntry& rhs){ return !(lhs > rhs); }
  inline bool operator>=(const IndexEntry& lhs, const IndexEntry& rhs){ return !(lhs < rhs); }

  /** This is the optimization we are interested in.
   A piece of machinery that can tell us quickly a group of points close to a given one.*/
class GeometryIndex {
  public:
    /** Put a point in the index, so that it can be used for lookups.
     * Don't use the mock Point class: if this works, can be put in the real code as is.*/
    void index(const Point& p, const PointIndex index);
    
    /** "Close" the index and allows lookups.
     Basically, sort its entries.
     Yes, we could have done some "automation" here (e. g. re-sort after index calls).
     But I keep it simple: I assume that there are a ton of points to index (too much re-sorting) and
     the lookup is on the critical path (don't waste time to see if the index is ready or not).
     I don't know if this is really critical, I would have to profile the client code.
     */
    void prepareForLookups();
    
    /** Put in closePoints the indexes of the points that are within the aabb of side 2 * DISTANCE centered on p.
     Notice that it only copy/push back in closePoints, so that the same vector can be reused
     by the caller. I did not test it, but maybe this permits to avoid re-allocating the vector at each call.
     
     Why 2 * distance?
     So that the box that contains fully the circle of centre p and radius distance.
     This is to avoid corner cases (literally: points in the corners of the aabb that get selected and win over
     closer points near the sides of the aabb).
     Once we have the points in the aabb, only those within distance are good.
     */
    void withinAabb(const Point& p, const double distance, std::vector<PointIndex>& closePoints) const;
    
private:
  std::vector<IndexEntry> indexX;
  std::vector<IndexEntry> indexY;
  std::vector<IndexEntry> indexZ;
};
  
  
}

#endif