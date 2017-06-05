#ifndef GEOINDEX_DOMAIN_ASSERTIONS
#define GEOINDEX_DOMAIN_ASSERTIONS

#include "Common.hpp"
#include "BasicGeometry.hpp"

namespace geoIndex {

/** Utilities for testing. */

/** Passes if pointIndex is in one of the elements of result, fails otherwise. */
void ASSERT_INDEX_PRESENT(const std::vector<IndexAndGeometry<Point>>& result, const PointTraits<Point>::index pointIndex);

}

#endif