#include "gtest/gtest.h"

#include <iostream>

#include "Support.hpp"
#include "ReferenceSolution.hpp"
#include "GeometryIndex.hpp"
#include "CubeIndex.hpp"

#include <ctime>

class PoorMansTimer{
public:
  PoorMansTimer(const std::string& message) :
  message(message)
  {
    begin = std::clock();
  }
  
  ~PoorMansTimer() {
    clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    
    std::cout << message << " took " << elapsed_secs << " seconds" << std::endl;
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

namespace geoIndex {
  static void fillRedGrid(std::vector<Point>& redGrid) {
    const uint32_t pointsToUse = 700000;
    redGrid.clear();
    redGrid.reserve(pointsToUse);
    for (uint32_t i = 0; i < pointsToUse; ++i)
      redGrid.push_back({fRand(), fRand(), fRand()});
  }
  
  
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
}