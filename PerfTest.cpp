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
    const double fMin = -1500;
    const double fMax = +1500;
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
 

template<uint32_t numberOfPoints>
static const std::vector<Point>& redMesh() {
    static std::vector<Point> redMesh;
    static bool ready = false;
    if (! ready)  {
        fillRedMesh(redMesh, numberOfPoints);
        ready = true;
        std::cout << "Built mesh of " << numberOfPoints << " points" << std::endl;
    }
    return redMesh;
}

TEST(performance, prepareInitFirstUse) {
    PoorMansTimer("Test the timer");
    redMesh<1000>();
    redMesh<10000>();
    redMesh<100000>();
    redMesh<200000>();
    redMesh<1000000>();
}


template<typename INDEX>
void singleLookupTest(INDEX index, const std::vector<Point>& redMesh, double distance) {
    {
        auto t = PoorMansTimer("Index preparation");
        BuildIndex(redMesh, index);
    }

    const Point lookupPoint{0, 0, 0};
    std::vector<IndexAndSquaredDistance<Point> > results;
    {
        auto t = PoorMansTimer("Lookup time");
        index.pointsWithinDistance(lookupPoint, distance, results);
    }

    std::cout << "Found points " << results.size() << std::endl; //Ask the compiler not to optimize-out the result. 
}

template<typename INDEX>
void multipleLookupTest(INDEX index, const std::vector<Point>& redMesh, const std::vector<Point>& greenMesh, double distance) {
    BuildIndex(redMesh, index);
/*
    for (const auto& p : greenMesh){
        KNearestNeighbor(re);
    }
    
    const Point lookupPoint{0, 0, 0};
    std::vector<IndexAndSquaredDistance<Point> > results;
    {
        auto t = PoorMansTimer("Lookup time");
        index.pointsWithinDistance(lookupPoint, distance, results);
    }

    std::cout << "Found points " << results.size() << std::endl; //Ask the compiler not to optimize-out the result. */
}




/* Variation in collection size. */
TEST(PerformanceTest, noIndex_1000) {
    NoIndex<Point> index;
    singleLookupTest(index, redMesh<1000>(), 100);
}

TEST(PerformanceTest, aabb_1000) {
    AabbIndex<Point> index;
    singleLookupTest(index, redMesh<1000>(), 100);
}

TEST(PerformanceTest, cube_1000) {
    CubeIndex<Point> index(10);
    singleLookupTest(index, redMesh<1000>(), 100);
}


TEST(PerformanceTest, noIndex_10000) {
    NoIndex<Point> index;
    singleLookupTest(index, redMesh<10000>(), 100);
}

TEST(PerformanceTest, aabb_10000) {
    AabbIndex<Point> index;
    singleLookupTest(index, redMesh<10000>(), 100);
}

TEST(PerformanceTest, cube_10000) {
    CubeIndex<Point> index(10);
    singleLookupTest(index, redMesh<10000>(), 100);
}


TEST(PerformanceTest, noIndex_100000) {
    NoIndex<Point> index;
    singleLookupTest(index, redMesh<100000>(), 100);
}

TEST(PerformanceTest, aabb_100000) {
    AabbIndex<Point> index;
    singleLookupTest(index, redMesh<100000>(), 100);
}

TEST(PerformanceTest, cube_100000) {
    CubeIndex<Point> index(10);
    singleLookupTest(index, redMesh<100000>(), 100);
}


TEST(PerformanceTest, noIndex_200000) {
    NoIndex<Point> index;
    singleLookupTest(index, redMesh<200000>(), 100);
}

TEST(PerformanceTest, aabb_200000) {
    AabbIndex<Point> index;
    singleLookupTest(index, redMesh<200000>(), 100);
}

TEST(PerformanceTest, cube_200000) {
    CubeIndex<Point> index(10);
    singleLookupTest(index, redMesh<200000>(), 100);
}

TEST(PerformanceTest, noIndex_1000000) {
    NoIndex<Point> index;
    singleLookupTest(index, redMesh<1000000>(), 100);
}

TEST(PerformanceTest, aabb_1000000) {
    AabbIndex<Point> index;
    singleLookupTest(index, redMesh<1000000>(), 100);
}

TEST(PerformanceTest, cube_1000000) {
    CubeIndex<Point> index(10);
    singleLookupTest(index, redMesh<1000000>(), 100);
}


/* Variation in search distance. */

TEST(PerformanceTest, noIndex_d1) {
    NoIndex<Point> index;
    singleLookupTest(index, redMesh<200000>(), 1);
}

TEST(PerformanceTest, aabb_d1) {
    AabbIndex<Point> index;
    singleLookupTest(index, redMesh<200000>(), 1);
}

TEST(PerformanceTest, cube_d1) {
    CubeIndex<Point> index(10);
    singleLookupTest(index, redMesh<200000>(), 1);
}


TEST(PerformanceTest, noIndex_d10) {
    NoIndex<Point> index;
    singleLookupTest(index, redMesh<200000>(), 10);
}

TEST(PerformanceTest, aabb_d10) {
    AabbIndex<Point> index;
    singleLookupTest(index, redMesh<200000>(), 10);
}

TEST(PerformanceTest, cube_d10) {
    CubeIndex<Point> index(10);
    singleLookupTest(index, redMesh<200000>(), 10);
}


TEST(PerformanceTest, noIndex_d50) {
    NoIndex<Point> index;
    singleLookupTest(index, redMesh<200000>(), 50);
}

TEST(PerformanceTest, aabb_d50) {
    AabbIndex<Point> index;
    singleLookupTest(index, redMesh<200000>(), 50);
}

TEST(PerformanceTest, cube_d50) {
    CubeIndex<Point> index(10);
    singleLookupTest(index, redMesh<200000>(), 50);
}

/*
TEST(PerformanceTest, noIndex_multipleLookup) {
    NoIndex<Point> index;
    singleLookupTest(index, redMesh<200000>(), 50);
}

TEST(PerformanceTest, aabb_d50) {
    AabbIndex<Point> index;
    singleLookupTest(index, redMesh<200000>(), 50);
}

TEST(PerformanceTest, cube_d50) {
    CubeIndex<Point> index(10);
    singleLookupTest(index, redMesh<200000>(), 50);
}

*/

}