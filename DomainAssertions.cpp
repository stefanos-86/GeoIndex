#include "DomainAssertions.hpp"

#include <algorithm>
#include "gtest/gtest.h"

using namespace std;

namespace geoIndex {

void ASSERT_INDEX_PRESENT(const std::vector<IndexAndDistance<Point>>& result, const Point::index_t pointIndex) {
  ASSERT_NE(end(result), find_if(begin(result),
                            end(result),
                            [pointIndex](const IndexAndDistance<Point>&result_i){
                                return result_i.pointIndex == pointIndex;})
           );
}

}