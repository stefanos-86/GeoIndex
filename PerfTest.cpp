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


TEST(PerformanceTest, double__100000_points__no_index__lookup_distance_10__field_size_2000) {
    NoIndex<Point> index;
    genericTest(index, redMesh<100000>());
}

TEST(PerformanceTest, double__100000_points__aabb__lookup_distance_10__field_size_2000) {
    AabbIndex<Point> index;
    genericTest(index, redMesh<100000>());
}

TEST(PerformanceTest, double__100000_points__cube__lookup_distance_10__field_size_2000) {
    CubeIndex<Point> index(10);
    genericTest(index, redMesh<100000>());
}

}