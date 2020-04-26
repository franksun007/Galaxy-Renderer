#ifndef __GALAXY_RENDERER_STAR_H__
#define __GALAXY_RENDERER_STAR_H__

#include <cmath>
#include <limits>

#include "Galaxy.h"
#include "OrbitCalculator.h"
#include "Vector.h"

class Star {
public:
  Star()
      : _i(0), m_theta(0), m_a(0), m_b(0),
        m_distance_to_center(std::numeric_limits<float>::max()
                             // 0
                             ),
        m_center(0, 0) {}

  // Without the sqrt
  float RelativeDistance(const Vec2D &pos) {
    const float x = pos.x - m_pos.x;
    const float y = pos.y - m_pos.y;
    return (x * x + y * y) / 10000;
  }

  void UpdateGalaxyIfCloser(Galaxy &other_galaxy, const int32_t star_type) {
    const float other_dis = RelativeDistance(other_galaxy.GetGalaxyCenter());
    if (m_distance_to_center > other_dis) {
      SetGalaxy(other_galaxy, star_type);
    }
  }

  // TODO(Frank): star_type to be a Enum
  void SetGalaxy(Galaxy &other_galaxy, const int32_t star_type) {

    switch (star_type) {
    // Star
    case 0: {
      m_a = other_galaxy.GetRandomRad();
      m_theta = 360 * other_galaxy.GetRandom();
      m_velTheta = other_galaxy.GetOrbitalVelocity(m_a);
      m_angle = other_galaxy.GetAngularOffset(m_a);
      m_b = m_a * other_galaxy.GetExcentricity(m_a);
      m_center = other_galaxy.GetGalaxyCenter();
      break;
    }
    // Dust
    case 1:
    case 2: {
      const float rad = other_galaxy.GetRad();
      const float x = 2 * rad * other_galaxy.GetRandom() - rad;
      const float y = 2 * rad * other_galaxy.GetRandom() - rad;
      m_a = sqrt(x * x + y * y);
      m_b = m_a * other_galaxy.GetExcentricity(m_a);
      m_theta = 360 * other_galaxy.GetRandom();
      m_velTheta = other_galaxy.GetOrbitalVelocity((m_a + m_b) / 2.0);
      m_angle = other_galaxy.GetAngularOffset(m_a);
      m_center = other_galaxy.GetGalaxyCenter();
      break;
    }
    }
  }

  const Vec2D &CalcXY(int pertN, float pertAmp) {
    m_pos = OrbitCalculator::Compute(m_angle, m_a, m_b, m_theta, m_center,
                                     pertN, pertAmp);
    m_distance_to_center = RelativeDistance(m_center);
    return m_pos;
  }

  int _i;

  float m_theta;    // position auf der ellipse
  float m_velTheta; // angular velocity
  float m_angle;    // Schräglage der Ellipse
  float m_a;        // 小半轴
  float m_b;        // 大半轴
  float m_temp;     // star temperature
  float m_mag;      // brigtness;
  float m_distance_to_center;
  Vec2D m_center; // center of the elliptical orbit
  Vec2D m_vel;    // Current velocity (calculated)
  Vec2D m_pos;    // current position in kartesion koordinates
};

#endif // __GALAXY_RENDERER_STAR_H__