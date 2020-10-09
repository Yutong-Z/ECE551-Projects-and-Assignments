#include "circle.hpp"

#include <cmath>
#include <cstdio>

Circle::Circle(Point init_center, double init_radius) :
    center(init_center),
    radius(init_radius) {
}

void Circle::move(double dx, double dy) {
  center.move(dx, dy);
}

double Circle::intersectionArea(const Circle & otherCircle) {
  const double PI = 3.141592653589793238463;
  double r1 = radius;
  double r2 = otherCircle.radius;
  double d = center.distanceFrom(otherCircle.center);
  if (d >= (r1 + r2)) {
    return 0;
  }
  else if (r1 >= r2 && d <= (r1 - r2)) {
    return PI * r2 * r2;
  }
  else if (r1 < r2 && d <= (r2 - r1)) {
    return PI * r1 * r1;
  }
  else {
    double d1 = (r1 * r1 - r2 * r2 + d * d) / 2 / d;
    double d2 = d - d1;
    return r1 * r1 * std::acos(d1 / r1) + r2 * r2 * std::acos(d2 / r2) -
           d1 * std::sqrt(r1 * r1 - d1 * d1) - d2 * std::sqrt(r2 * r2 - d2 * d2);
  }
}
