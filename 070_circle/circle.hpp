#include "point.hpp"
class Circle {
  Point center;
  const double radius;

 public:
  Circle(Point init_center, double init_radius);
  void move(double dx, double dy);
  double intersectionArea(const Circle & otherCircle);
};
