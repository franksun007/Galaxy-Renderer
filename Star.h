#ifndef __GALAXY_RENDERER_STAR_H__
#define __GALAXY_RENDERER_STAR_H__

#include "OrbitCalculator.h"
#include "Vector.h"

class Star {
public:
  Star() : m_theta(0), m_a(0), m_b(0), m_center(0, 0) {}

  const Vec2D &CalcXY(int pertN, float pertAmp) {
    m_pos = OrbitCalculator::Compute(m_angle, m_a, m_b, m_theta, m_center,
                                     pertN, pertAmp);
    return m_pos;
  }

  float m_theta;    // position auf der ellipse
  float m_velTheta; // angular velocity
  float m_angle;    // Schräglage der Ellipse
  float m_a;        // kleine halbachse
  float m_b;        // große halbachse
  float m_temp;     // star temperature
  float m_mag;      // brigtness;
  Vec2D m_center;    // center of the elliptical orbit
  Vec2D m_vel;       // Current velocity (calculated)
  Vec2D m_pos;       // current position in kartesion koordinates
};

#endif // __GALAXY_RENDERER_STAR_H__