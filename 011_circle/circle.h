#include "point.h"

class Circle {
 private:
  Point center;
  const double radius;

 public:
  Circle(Point init_center, const double init_radius) :
      center(init_center),
      radius(init_radius){};
  void move(double dx, double dy);
  double intersectionArea(const Circle & otherCircle);
};
