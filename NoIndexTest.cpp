#include "gtest/gtest.h"

#include "NoIndex.hpp"

#include <algorithm>
#include <vector>
#include <limits>

#include "BasicGeometry.hpp"

using namespace std;

namespace geoIndex {

template <typename COLLECTION, typename T>
void ASSERT_PRESENT(const COLLECTION& c, const T element) {
  ASSERT_NE(end(c), find(begin(c), end(c), element));
}
  
TEST(NoIndex, Construction_unknownSize) {
  NoIndex<Point>();  // Ok it it compiles - can't test the internal collection (well, could - with a memory profiler).
}

TEST(NoIndex, Construction_withSize) {
  NoIndex<Point>(25);  // Ok it it compiles - can't test the internal collection (well, could - with a memory profiler).
}

TEST(NoIndex, pointsWithin_samePoint) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t referenceIndex = 1;
  
  NoIndex<Point> redMesh(1);
  redMesh.index(referencePoint, 1);
  
  std::vector<Point::index_t> result;
  redMesh.pointsWithin(referencePoint, 0.01, result);
  
  ASSERT_PRESENT(result, referenceIndex);
}

TEST(NoIndex, pointsWithin_coincidentPoints) {
  const Point referencePoint{1, 55, 2};
  
  NoIndex<Point> redMesh(2);
  redMesh.index(referencePoint, 1);
  redMesh.index(referencePoint, 2);
  
  std::vector<Point::index_t> result;
  redMesh.pointsWithin(referencePoint, 0.01, result);
  
  ASSERT_PRESENT(result, 1);
  ASSERT_PRESENT(result, 2);
}

TEST(NoIndex, pointsWithin_noPoints) {
  const Point referencePoint{0, 0, 0};
  
  NoIndex<Point> redMesh(2);
  
  std::vector<Point::index_t> result;
  redMesh.pointsWithin(referencePoint, 0.01, result);
  
  ASSERT_TRUE(result.empty());
}

TEST(NoIndex, pointsWithin_onlyFarPoints) {
  const Point referencePoint{0, 0, 0};
  const Point far1{100, 0, 0};
  const Point far2{154, 5, 0};
  const Point far3{100, 0, 256};
  
  NoIndex<Point> redMesh(3);
  redMesh.index(far1, 1);
  redMesh.index(far2, 2);
  redMesh.index(far3, 3);
  
  std::vector<Point::index_t> result;
  redMesh.pointsWithin(referencePoint, 0.01, result);
  
  ASSERT_TRUE(result.empty());
}

TEST(NoIndex, pointsWithin_inAndOutPoints) {
  const Point referencePoint{0, 0, 0};
  const Point pIn{99.99, 0, 0};
  const Point pOut{101.1, 0, 0};
  
  NoIndex<Point> redMesh(2);
  redMesh.index(pIn, 1);
  redMesh.index(pOut, 2);
  
  std::vector<Point::index_t> result;
  redMesh.pointsWithin(referencePoint, 101, result);
  
  ASSERT_EQ(1, result.size());
  ASSERT_PRESENT(result, 1);
}

TEST(NoIndex, pointsWithin_exactDistance) {
  const Point referencePoint{0, 0, 0};
  const Point onTheBorder{100, 0, 0};
  
  NoIndex<Point> redMesh(1);
  redMesh.index(onTheBorder, 1);
  
  std::vector<Point::index_t> result;
  redMesh.pointsWithin(referencePoint, 100, result);
  
  ASSERT_TRUE(result.empty());
}

/*
testare anche il caso di punti alla distanza esatta, subito dentro, subito fuori
poi commit
poi la generica funzione per il 2 neighbor
poi fare gli altri tipi di indici
poi test di performance in tutte le salse.
*/

#ifdef GEO_INDEX_SAFETY_CHECKS
TEST(NoIndex, index_duplicatedIndex) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t sameIndex = 1;
  
  NoIndex<Point> redMesh(2);
  redMesh.index(referencePoint, sameIndex);
  
  ASSERT_ANY_THROW(redMesh.index(referencePoint, sameIndex));
}

TEST(NoIndex, pointsWithin_negativeDistance) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t index = 1;
  
  NoIndex<Point> redMesh(1);
  redMesh.index(referencePoint, index);
  
  std::vector<Point::index_t> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithin(referencePoint, -1, result););
}

TEST(NoIndex, pointsWithin_zeroDistance) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t index = 1;
  
  NoIndex<Point> redMesh(1);
  redMesh.index(referencePoint, index);
  
  std::vector<Point::index_t> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithin(referencePoint, 0, result););
}

TEST(NoIndex, pointsWithin_NanDistance) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t index = 1;
  
  NoIndex<Point> redMesh(1);
  redMesh.index(referencePoint, index);
  
  std::vector<Point::index_t> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithin(referencePoint, std::numeric_limits<double>::quiet_NaN(), result););
}

TEST(NoIndex, pointsWithin_overflowDistance) {
  const Point referencePoint{1, 55, 2};
  const Point::index_t index = 1;
  
  NoIndex<Point> redMesh(1);
  redMesh.index(referencePoint, index);
  
  std::vector<Point::index_t> result;
  
  ASSERT_ANY_THROW(redMesh.pointsWithin(referencePoint, std::numeric_limits<double>::max(), result););
}

#endif

}