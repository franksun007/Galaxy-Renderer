#ifndef __GALAXY_RENDERER_ORBIT_CALCULATOR_H__
#define __GALAXY_RENDERER_ORBIT_CALCULATOR_H__

#include "Vector.h"

class OrbitCalculator {

public:
  static Vec2D Compute(double angle, double a, double b, double theta,
                       const Vec2D &p, int pertN, double pertAmp);
};

#endif // __GALAXY_RENDERER_ORBIT_CALCULATOR_H__