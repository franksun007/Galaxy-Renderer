#include "OrbitCalculator.h"

#include <cmath>

#include "Constants.h"

using namespace std;

Vec2D OrbitCalculator::Compute(double angle, double a, double b, double theta,
                               const Vec2D &p, int pertN, double pertAmp) {
  const double beta = -angle;
  const double alpha = theta * constants::DEG_TO_RAD;

  // temporaries to save cpu time
  const double cosalpha = cos(alpha);
  const double sinalpha = sin(alpha);
  const double cosbeta = cos(beta);
  const double sinbeta = sin(beta);

  Vec2D pos = Vec2D(p.x + (a * cosalpha * cosbeta - b * sinalpha * sinbeta),
                    p.y + (a * cosalpha * sinbeta + b * sinalpha * cosbeta));

  // Add small perturbations to create more spiral arms
  if (pertAmp > 0 && pertN > 0) {
    pos.x += (a / pertAmp) * sin(alpha * 2 * pertN);
    pos.y += (a / pertAmp) * cos(alpha * 2 * pertN);
  }

  return pos;
}