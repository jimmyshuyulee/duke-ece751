#include "circle.h"

#include <algorithm>
#include <cmath>

void Circle::move(double dx, double dy) {
  center.move(dx, dy);
}
double Circle::intersectionArea(const Circle & otherCircle) {
  if (this == &otherCircle) {
    return M_PI * pow(radius, 2);
  }

  double d = center.distanceFrom(otherCircle.center);
  if (d > radius + otherCircle.radius) {
    return 0.0;
  }

  double r1_sq = pow(radius, 2);
  double r2_sq = pow(otherCircle.radius, 2);

  if (d < abs(radius - otherCircle.radius)) {
    return M_PI * std::min(r1_sq, r2_sq);
  }
  double d1 = (r1_sq - r2_sq + pow(d, 2)) / (2 * d);
  double d2 = (r2_sq - r1_sq + pow(d, 2)) / (2 * d);

  return r1_sq * acos(d1 / radius) - d1 * sqrt(r1_sq - pow(d1, 2)) +
         r2_sq * acos(d2 / otherCircle.radius) - d2 * sqrt(r2_sq - pow(d2, 2));
}

