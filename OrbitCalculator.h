#ifndef __GALAXY_RENDERER_ORBIT_CALCULATOR_H__
#define __GALAXY_RENDERER_ORBIT_CALCULATOR_H__

#include "Vector.h"

class OrbitCalculator {

public:
  static Vec2D Compute(float angle, float a, float b, float theta,
                       const Vec2D &p, int pertN, float pertAmp);
};

#endif // __GALAXY_RENDERER_ORBIT_CALCULATOR_H__