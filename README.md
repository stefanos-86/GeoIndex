# GeoIndex
Or: the [k-nearest-neighbor](https://en.wikipedia.org/wiki/Nearest_neighbor_search) for dummies.

Some code to find the closest points in space to a given reference point, done on the cheap.
This code is meant to be good enough for an inexperienced programmer to use and to be decently fast.
No, it is nowhere near the optimal solution, and its design/coding/speed/anything can certainly be improved. I warned you.

## How to use
* Dowload the code on Linux.
* cmake .  (mind the '.'!)
* make

Requires GTest and its dependencies. 
Requires Boost as well, but only for one of the algorithms. Just delete it if you can't have Boost.

Make will give 4 executables with the unit and performance tests.
The *_checked variants have self-check assertions inside (for some overflows, indexing errors...) but are a lot slower.

## How to use in your code
Just copy the .hpp files where you need them (or add to the compiler search path).

Compile with -DGEO_INDEX_SAFETY_CHECKS to get the "paranoia" version like in the *_checked tests. (like I do inside CMakeLists.txt).

Have a look in NearestNeighborsTests.cpp for usage examples.
By and large:

```cpp
std::vector<Point> points; // You must fill this with your points.
std::vector<IndexAndSquaredDistance<Point> > result;  // Points have indices that act as their "names".
                                                      // The indices used to build the faces of a mesh are perfect for this.

NoIndex<Point> geometryIndex;  // Or AabbIndex, or CubeIndex
BuildIndex(points, geometryIndex);
const Point referencePoint{0, 0, 0};  // Or any point you need the neighbors of.

const PointTraits<Point>::coordinate cullingDistance = 65; // How far from referencePoint can we look for neighbors?
                                                           // The less, the fastest. The more, the easier it is to find points.

const size_t k = 2;  // How many near points to look for.

KNearestNeighbor(
    geometryIndex,
    cullingDistance,
    referencePoint,
    k,
    result);

```
BuildIndex wants points to have the points in index order. If not the case, just loop on them and call `geometryIndex.index(point, index)`.

In the end `result` contains the index of the k points closest to the reference (or less, if it could not find enough points
within the culling distance). The points are sorted from the most close to the most far.

Each point index comes with the squared distance form the reference point (...it's a cheap optimization so save a lot
of square roots internally).


## Which algorithm should I use?
Whichever is fastest on your data.

The speed depends on what you feed to the algorithms (are the points clustered togheter? Very distant?...).

There are 5 possibilities. They all work the same, like in the example above.
Check the comments above the methods in the classes for more details.

0. NoIndex<...>, simple brute-force method. It can be fast enough.
0. AabbIndex<...>, takes the points in the "axis aligned bounding box" around the reference. ...slower than the brute force method. My implementation must be very poor.

0 PermutationAabbIndex<...>, same as AabbIndex with different internal data structures. It is even worst.
0. CubeIndex<...>, the fastest (in my tests!). A "voxel style" method that groups the points in cubes, then just works in the "right" cubes. Careful with the constructor parameter (cube size): too big, and it can't discard many useless points; too small and it has to work on too many cubes.
0. BoostIndex<...> is just a wrapper around [Boost spatial indexes](https://www.boost.org/doc/libs/1_69_0/libs/geometry/doc/html/geometry/spatial_indexes.html) to have a comparison with the "state of art". It takes ages to build the indexes, but it is 10 times faster than anything else when doing a lookup. You should NOT use this one... I mean, you have Boost alredy, just use it directly! 

Don't forget to time how long does it take to prepare the index! It may "eat" all you gain with faster searches.

## Bells and whistles!

Point uses double for geometry and coordinates and size_t for point indexes.
If it does not suit you, or you already have a class for points (maybe from a real-world library...), you can use it.
Specialize geoIndex::PointTraits for it (look for the "TEST(KNearestNeighbor, UserDefinedClasses) " in NearestNeighborsTest.cpp and copy from there).

## Acknowledgments
I would like to thank Alessio Castorrini (for challenging me to solve this problem and for testing the result) and [Marco Arena](https://github.com/ilpropheta) (for pulling me out of a nasty template trap I put myself into). 

