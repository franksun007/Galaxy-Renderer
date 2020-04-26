#include "OrbitCalculator.h"

#include <cmath>

#include "Constants.h"

using namespace std;

Vec2D OrbitCalculator::Compute(float angle, float a, float b, float theta,
                               const Vec2D &p, int pertN, float pertAmp) {
  const float beta = -angle;
  const float alpha = theta * constants::DEG_TO_RAD;

  const float cosalpha = cos(alpha);
  const float sinalpha = sin(alpha);
  const float cosbeta = cos(beta);
  const float sinbeta = sin(beta);

  Vec2D pos = Vec2D(p.x + (a * cosalpha * cosbeta - b * sinalpha * sinbeta),
                    p.y + (a * cosalpha * sinbeta + b * sinalpha * cosbeta));

  // Add small perturbations to create more spiral arms
  if (pertAmp > 0 && pertN > 0) {
    pos.x += (a / pertAmp) * sin(alpha * 2 * pertN);
    pos.y += (a / pertAmp) * cos(alpha * 2 * pertN);
  }

  return pos;
}