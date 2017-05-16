#ifndef SUPPORT_H
#define SUPPORT_H

/** Support code that we need for demo purposes.
 Since I don't have the full code, I need to replace ("mock") the parts I don't have.
 In most cases those are basic implementations. May not be fast, safe, optimal...
 */

namespace geoIndex {

/** Point in 3D space, as simple as it can be. */
struct Point {
  double x;
  double y;
  double z;
};

/** Computes the distance of two points. */
double Distance(const Point& p1, const Point& p2);

}

#endif