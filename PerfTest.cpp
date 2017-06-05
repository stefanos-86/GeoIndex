#include "gtest/gtest.h"

#include <iostream>
#include <ctime>

#include "NoIndex.hpp"
#include "AabbIndex.hpp"
#include "CubeIndex.hpp"

#include "NearestNeighbors.hpp"

namespace geoIndex {
    
class PoorMansTimer{
public:
    PoorMansTimer(const std::string& message) :
    message(message),
    begin(std::clock()) { }

    ~PoorMansTimer() {
        clock_t end = std::clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        std::cout << message << ": " << elapsed_secs<< std::endl;
}
  
private:
  const std::string message;
  clock_t begin;

};


//From http://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c
double fRand()
{
    const double fMin = -2000;
    const double fMax = +2000;
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

/* This code will never be timed. The user already has a grid (we are just making one for testing). */
static void fillRedMesh(std::vector<Point>& redMesh, const uint32_t pointsToUse) {
    redMesh.clear();
    redMesh.reserve(pointsToUse);
    for (uint32_t i = 0; i < pointsToUse; ++i)
        redMesh.push_back({fRand(), fRand(), fRand()});
    
}
 
TEST(performance, timingOverhead) {
    PoorMansTimer("Test the timer");
}

template<uint32_t numberOfPoints>
static const std::vector<Point>& redMesh() {
    static std::vector<Point> redMesh;
    static bool ready = false;
    if (! ready)  {
        fillRedMesh(redMesh, numberOfPoints);
        ready = true;
    }
    
    std::cout << "Built mesh of " << numberOfPoints << " points" << std::endl;
    return redMesh;
}

template<typename INDEX>
void genericTest(INDEX index, const std::vector<Point>& redMesh) {
    {
        auto t = PoorMansTimer("Index preparation");
        BuildIndex(redMesh, index);
    }

    const Point lookupPoint{0, 0, 0};
    std::vector<IndexAndSquaredDistance<Point> > results;
    {
        auto t = PoorMansTimer("Lookup time");
        index.pointsWithinDistance(lookupPoint, 500, results);
    }

    std::cout << "Found points " << results.size() << std::endl; //Ask the compiler not to optimize-out the result. 
}


TEST(PerformanceTest, double__1000_points__no_index__lookup_distance_10__field_size_2000) {
    NoIndex<Point> index;
    genericTest(index, redMesh<1000>());
}

TEST(PerformanceTest, double__1000_points__aabb__lookup_distance_10__field_size_2000) {
    AabbIndex<Point> index;
    genericTest(index, redMesh<1000>());
}

TEST(PerformanceTest, double__1000_points__cube__lookup_distance_10__field_size_2000) {
    CubeIndex<Point> index(10);
    genericTest(index, redMesh<1000>());
}


TEST(PerformanceTest, double__10000_points__no_index__lookup_distance_10__field_size_2000) {
    NoIndex<Point> index;
    genericTest(index, redMesh<10000>());
}

TEST(PerformanceTest, double__10000_points__aabb__lookup_distance_10__field_size_2000) {
    AabbIndex<Point> index;
    genericTest(index, redMesh<10000>());
}

TEST(PerformanceTest, double__10000_points__cube__lookup_distance_10__field_size_2000) {
    CubeIndex<Point> index(10);
    genericTest(index, redMesh<10000>());
}

//MALEDIZIONE!!! I 3 algoritmi non ritornano lo stesso risultato


    /*

  
  TEST(performance, base) { // 450 to 500 ms on my machine.
    PoorMansTimer fullTest("baseline whole test");
    std::vector<Point> redGrid;
    fillRedGrid(redGrid);
    Point greenPoint{0, 0, 0};
    
    { PoorMansTimer t("baseline lookups");
    const auto result = TwoClosestPoints(greenPoint, redGrid);
    std::cout << result.first << result.second << std::endl; // Use stuff or it may be optimized out.
    }
  }
  
  TEST(performance, aabbCulling) {  // 1000ms... Does not work!!!
    PoorMansTimer fullTest("aabb whole test");
    std::vector<Point> redGrid;
    fillRedGrid(redGrid);
    Point greenPoint{0, 0, 0};
    
    GeometryIndex gi;
    for (size_t i = 0; i < redGrid.size(); ++i)
      gi.index(redGrid[i], i);
    gi.prepareForLookups();
    
    { PoorMansTimer t("aabb lookups");
    std::vector<PointIndex> closePointsIndexes;
    gi.withinAabb(greenPoint, 100.0, closePointsIndexes);
    
    
    std::vector<Point> closePoints;
    closePoints.reserve(closePointsIndexes.size());
    for (const auto i : closePointsIndexes)
      closePoints.push_back(redGrid[i]);
    // TODO: check stuff is within a sphere to avoid points in the corners...
    // TODO: make a variant of TwoClosestPoints that takes the red grid and the indexes.
    
    std::cout << "Points from aabb: " << closePoints.size() << std::endl;
    // Now do the test, but only on a few points.
    const auto result = TwoClosestPoints(greenPoint, closePoints);
    std::cout << result.first << result.second << std::endl; // Use stuff or it may be optimized out.
    }
  }
  
  TEST(performance, CubeIndex) {
    PoorMansTimer fullTest("cubes whole test");
    std::vector<Point> redGrid;
    fillRedGrid(redGrid);
    Point greenPoint{0, 0, 0};
    
    CubeIndex cu(500);
    for (size_t i = 0; i < redGrid.size(); ++i)
      cu.addOnePoint(redGrid[i], i);
    
    { PoorMansTimer t("cubes lookups");
      std::vector<PointIndex> closePointsIndexes;
      cu.closeTo(greenPoint, closePointsIndexes);
      
      std::vector<Point> closePoints;
      closePoints.reserve(closePointsIndexes.size());
      for (const auto i : closePointsIndexes)
        closePoints.push_back(redGrid[i]);
      // Same considerations about checking a sphere as the aabb culling...
     
    std::cout << "Points from cubes: " << closePoints.size() << std::endl;
    const auto result = TwoClosestPoints(greenPoint, closePoints);
    std::cout << result.first << result.second << std::endl; // Use stuff or it may be optimized out.
    }
  }
  */
}