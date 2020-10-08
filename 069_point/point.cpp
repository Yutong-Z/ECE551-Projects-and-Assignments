#include "point.hpp"

#include <cmath>
#include <cstdio>

Point::Point() : x(0), y(0) {
}

void Point::move(double dx, double dy) {
  x = x + dx;
  y = y + dy;
}

double Point::distanceFrom(const Point & p) {
  double dx = p.x - x;
  double dy = p.y - y;
  return std::sqrt(dx * dx + dy * dy);
}
