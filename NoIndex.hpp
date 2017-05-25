#ifndef GEOINDEX_NO_INDEX
#define GEOINDEX_NO_INDEX

#include <vector>

#include "BasicGeometry.hpp"

#ifdef GEO_INDEX_SAFETY_CHECKS
  #include <algorithm>
  #include <stdexcept>
  #include <cmath>
#endif

namespace geoIndex {

/** This index implementation... does not do any indexing. 
 *  It is just a collection of points. 
 TODO: do not forget to annotate the required methods in the function that uses the various indexes.*/

template <typename POINT>
class NoIndex
{
public: 
  /** If you know how many points you are going to use, tell it to this constructor to 
   *  reserve memory. */
  NoIndex(const size_t expectedCollectionSize = 0) {
    points.reserve(expectedCollectionSize);
  }
  
  /** Adds a point to the index. Remember its name too. */
  void index(const POINT& p, const typename POINT::index_t index){
#ifdef GEO_INDEX_SAFETY_CHECKS
    if (std::find(begin(indices), end(indices), index) != end(indices))
      throw std::runtime_error("NoIndex::index Point indexed twice");
#endif
    
    points.push_back(p);
    indices.push_back(index);
  }
  
  /** Does nothing, since this is not a real index.
   *  Kept to ensure it has the same interface as the others. */
  void completed() {};
  
  /** Finds the points that are within distance d from p. Cleans the output vector before filling it.
   *  Returns the points sorted in distance order from p (to simplify computing the k-nearest-neighbor).
   * 
   *  And it does it with pure brute force: check p against all other points.
   *  This is the simplest possible algorithm.
   * 
   *  Returns only points strictly within the distance.
   * 
   *  TODO: try to make it better: take an iterator to a generic collection. Also in the other implementations. */
  void pointsWithin(const POINT& p, 
                    const typename POINT::coordinate_t d,
                    std::vector<typename POINT::index_t>& output) const {
#ifdef GEO_INDEX_SAFETY_CHECKS
    if (d <= 0)
      throw std::runtime_error("NoIndex::pointsWithin Negative distance");
    if (std::isnan(d))
      throw std::runtime_error("NoIndex::pointsWithin Invalid distance");
#endif
                      
        output.clear();
        const typename Point::coordinate_t distanceLimit = d * d;  // Don't forget we use squared distances.
                                                                   // TODO: if we call this a lot of time, better have an overload that takes the square...
                                                                   
#ifdef GEO_INDEX_SAFETY_CHECKS
    if (std::isinf(distanceLimit))
      throw std::runtime_error("NoIndex::pointsWithin Overflow");
#endif
        
        for (size_t i = 0; i < points.size(); ++i)
          if (SquaredDistance(p, points[i]) < distanceLimit)
            output.push_back(indices[i]);
    
                      
    }
                    
private:
  // Internal data: parallel arrays.
  std::vector<POINT> points;
  std::vector<typename POINT::index_t> indices;
};

}


#endif