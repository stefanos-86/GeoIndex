#ifndef GEOINDEX_BOOST_INDEX
#define GEOINDEX_BOOST_INDEX

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/register/point.hpp> 

#include "Common.hpp"

/** Boost goemetry is somewhat shitty when it comes to namespace.
    I can't figure out what is wrong if the class and the macro are in the geoIndex namespace.
    Workaround with this setup... I won't even try to use the point traits.*/
class geoIndex_PointWithIndex {
public:
    geoIndex_PointWithIndex(double x, double y, double z, size_t index) :
        x(x), y(y), z(z), index(index) {};
    double x, y, z;
    size_t index;
};
BOOST_GEOMETRY_REGISTER_POINT_3D(geoIndex_PointWithIndex, double, boost::geometry::cs::cartesian, x, y, z);

typedef boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian> boostPoint;
typedef boost::geometry::model::box<boostPoint> boostBox;

namespace geoIndex {


    
/** This is the real deal, the spatial index from Boost Geomtry, re-packaged to offer the same
 *  interface as the other indexes.
 *  
 *  I had troubles to pass the usual point class and point traits into boost's templates.
 *  Expect several "shortcuts" in the implementation of this class.
 * 
 *  Also expect less paranoid tests: I just wrapped a well known library so it would fit with the rest of the
 *  project and be called like the other algorithms. I have no doubts boost's code is perfectly working.
 * 
 *  Index construction is slower, but lookups are 10 times faster.
 *  It could probably be possible to increase the performance even further, but I did not check all the
 *  possible ways of tweaking the r-tree.
 */
template <typename POINT>
class BoostIndex
{
public: 
      
  /** Adds a point to the index. Remember its name too. */
  void index(const POINT& p, const typename PointTraits<POINT>::index index){
      rtreeIndex.insert(geoIndex_PointWithIndex(p.x, p.y, p.z, index));
  }
  
    /** Does nothing, boost has no such step, but I keep the interface. */
    void completed() {};
  
  
  /** Finds the points that are within distance d from p. Cleans the output vector before filling it.
   *  Returns the points sorted in distance order from p (to simplify computing the k-nearest-neighbor).
   *  The returned structure also gives the squared distance. The client can do a sqrt and use it for its computations.
   * 
   *  Calls the boost index.
   * 
   *  Returns only points strictly within the distance.
   */
    void pointsWithinDistance(const POINT& p, 
                              const typename PointTraits<POINT>::coordinate d,
                              std::vector<IndexAndSquaredDistance<POINT> >& output) const {

  
    const double distanceLimit = d * d;
    
    const boostPoint top(p.x + d, p.y + d, p.z + d);
    const boostPoint bottom(p.x - d, p.y - d, p.z - d);
    const boostBox queryBox(bottom, top);
    
    std::vector<geoIndex_PointWithIndex> inBoundingBox;
    rtreeIndex.query(boost::geometry::index::intersects(queryBox), std::back_inserter(inBoundingBox));
  
    output.clear();
    for(const  geoIndex_PointWithIndex& candidateFromBox : inBoundingBox) {
        POINT candidate;
        candidate.x = candidateFromBox.x;
        candidate.y = candidateFromBox.y;
        candidate.z = candidateFromBox.z;
        
        const auto squaredDistance = SquaredDistance(p, candidate);
        if (squaredDistance < distanceLimit)
            output.push_back({candidateFromBox.index, squaredDistance});
    }
    
    std::sort(std::begin(output), std::end(output), SortByGeometry<POINT>);
    
  }
                    
private:
    
    
    
  boost::geometry::index::rtree<
        geoIndex_PointWithIndex , 
        boost::geometry::index::linear<16>  // quadratic and rstar are faster, but creating the index takes so long it is not convenient. 
    > rtreeIndex;
};



}


#endif
