#include "gtest/gtest.h"

#include <iostream>
#include <ctime>
#include <sstream>

#include "NoIndex.hpp"
#include "AabbIndex.hpp"
#include "CubeIndex.hpp"
#include "PermutationAabbIndex.hpp"
#include "BoostIndex.hpp"

#include "NearestNeighbors.hpp"

namespace geoIndex {
  
    
class PoorMansTimerString{
public:
    PoorMansTimerString() :
    begin(std::clock()) { }
    
    double stop() {
        clock_t end = std::clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        
        return elapsed_secs;
    }
    
private:
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
    redMesh<1000>();
    redMesh<10000>();
    redMesh<100000>();
    redMesh<200000>();
    redMesh<1000000>();
}


void tableHeader() {
    printf("%20s|%20s|%20s|%20s|%20s\n", "Mesh size", "index preparation", "point lookup", "found points", "search distance");
}

template<typename INDEX>
void singleLookupTest_tabulated(INDEX index, const std::vector<Point>& redMesh, double distance) {
    
        PoorMansTimerString tPreparation;
        BuildIndex(redMesh, index);
        double indexPreparation = tPreparation.stop();
    

    const Point lookupPoint{0, 0, 0};
    std::vector<IndexAndSquaredDistance<Point> > results;
    
        PoorMansTimerString tLookup;
        index.pointsWithinDistance(lookupPoint, distance, results);
        double lookup = tLookup.stop();
    
    printf("%20lu|%20f|%20f|%20lu|%20f\n", redMesh.size(), indexPreparation, lookup, results.size(), distance);
}

template<typename INDEX>
void multipleLookupTest(INDEX index, const std::vector<Point>& redMesh, const std::vector<Point>& greenMesh, double distance) {
    BuildIndex(redMesh, index);
    static const size_t neededNearest = 2;

    std::vector<IndexAndSquaredDistance<Point> > results;
    
        PoorMansTimerString t;
        for (const auto& p : greenMesh)
            KNearestNeighbor(index, distance, p, neededNearest, results);
        double elapsed = t.stop();
        
        printf("Red mesh size: %20lu, Green mesh size: %20lu, distance, %20f, points found %20lu, time %20f\n",
               redMesh.size(), greenMesh.size(), distance, results.size(), elapsed);
}


TEST(PerformanceTest, collectionSize_noIndex) {
    tableHeader();
    {
        NoIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<1000>(), 100);
    }
    {
        NoIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<10000>(), 100);
    }
    {
        NoIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<100000>(), 100);
    }
    {
        NoIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 100);
    }
    {
        NoIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<1000000>(), 100);
    }
    
    std::cout << std::endl;
}


TEST(PerformanceTest, collectionSize_aabb) {
    tableHeader();
    {
        AabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<1000>(), 100);
    }
    {
        AabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<10000>(), 100);
    }
    {
        AabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<100000>(), 100);
    }
    {
        AabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 100);
    }
    {
        AabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<1000000>(), 100);
    }
    
    std::cout << std::endl;
}


TEST(PerformanceTest, collectionSize_cube) {
    tableHeader();
    {
        CubeIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<1000>(), 100);
    }
    {
        CubeIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<10000>(), 100);
    }
    {
        CubeIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<100000>(), 100);
    }
    {
        CubeIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 100);
    }
    {
        CubeIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<1000000>(), 100);
    }
    
    std::cout << std::endl;
}


TEST(PerformanceTest, collectionSize_aabbWithPermutation) {
    tableHeader();
    {
        PermutationAabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<1000>(), 100);
    }
    {
        PermutationAabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<10000>(), 100);
    }
    {
        PermutationAabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<100000>(), 100);
    }
    {
        PermutationAabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 100);
    }
    {
        PermutationAabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<1000000>(), 100);
    }
    
    std::cout << std::endl;
}

TEST(PerformanceTest, collectionSize_boost) {
    tableHeader();
    {
        BoostIndex<Point> index;
        singleLookupTest_tabulated(index, redMesh<1000>(), 100);
    }
    {
        BoostIndex<Point> index;
        singleLookupTest_tabulated(index, redMesh<10000>(), 100);
    }
    {
        BoostIndex<Point> index;
        singleLookupTest_tabulated(index, redMesh<100000>(), 100);
    }
    {
        BoostIndex<Point> index;
        singleLookupTest_tabulated(index, redMesh<200000>(), 100);
    }
    printf("%20s|%s\n", "1000000", "not tested - index preparation too long");
   /* {
        BoostIndex<Point> index;
        singleLookupTest_tabulated(index, redMesh<1000000>(), 100);
    }*/
    
    std::cout << std::endl;
}

TEST(PerformanceTest, searchDistance_noIndex) {
    tableHeader();
    {
        NoIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 1);
    }
    {
        NoIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 10);
    }
    {
        NoIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 50);
    }
   
    std::cout << std::endl;
}

TEST(PerformanceTest, searchDistance_aabb) {
    tableHeader();
    {
        AabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 1);
    }
    {
        AabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 10);
    }
    {
        NoIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 50);
    }
   
    std::cout << std::endl;
}


TEST(PerformanceTest, searchDistance_cube) {
    tableHeader();
    {
        CubeIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 1);
    }
    {
        CubeIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 10);
    }
    {
        CubeIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 50);
    }
   
    std::cout << std::endl;
}

TEST(PerformanceTest, searchDistance_permutation) {
    tableHeader();
    {
        PermutationAabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 1);
    }
    {
        PermutationAabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 10);
    }
    {
        PermutationAabbIndex<Point> index(10);
        singleLookupTest_tabulated(index, redMesh<200000>(), 50);
    }
   
    std::cout << std::endl;
}


TEST(PerformanceTest, searchDistance_boost) {
    tableHeader();
    printf("(Reduced mesh size - index creation too slow to stand...)\n");
    { 
        BoostIndex<Point> index;
        singleLookupTest_tabulated(index, redMesh<100000>(), 1);
    }
    {
        BoostIndex<Point> index;
        singleLookupTest_tabulated(index, redMesh<100000>(), 10);
    }
    {
        BoostIndex<Point> index;
        singleLookupTest_tabulated(index, redMesh<100000>(), 50);
    }
   
    std::cout << std::endl;
}



/* Multiple lookups test: check for caching effects. */
TEST(PerformanceTest, multipleLookups) {
    { 
        printf ("no index - ");
        NoIndex<Point> index;
        multipleLookupTest(index, redMesh<200000>(), redMesh<1000>(), 30);
    }
    { 
        printf ("aabb - ");
        AabbIndex<Point> index(10);
        multipleLookupTest(index, redMesh<200000>(), redMesh<1000>(), 30);
    }
    { 
        printf ("cube - ");
        CubeIndex<Point> index(10);
        multipleLookupTest(index, redMesh<200000>(), redMesh<1000>(), 30);
    }
    { 
        printf ("permutation - ");
        PermutationAabbIndex<Point> index;
        multipleLookupTest(index, redMesh<200000>(), redMesh<1000>(), 30);
    }
    { 
        printf ("boost - ");
        BoostIndex<Point> index;
        multipleLookupTest(index, redMesh<200000>(), redMesh<1000>(), 30);
    }

    std::cout << std::endl;
}

}
