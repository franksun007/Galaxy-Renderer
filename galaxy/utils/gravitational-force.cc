
#include "gravitational-force.h"

namespace galaxy {
namespace {
// TODO(Frank): float might not be enough
constexpr float kGravitationalConstant = 6.674 * 0.00000000001;

} // namespace

float ScaledGravitationalForce(const float mass_a, const float mass_b,
                               const float distance, const float scale) {
  const float inverted_distance = 1.0f / distance;
  return mass_a * inverted_distance * mass_b * inverted_distance * scale *
         kGravitationalConstant;
}

float RelativeGravitationalForce(const float mass_a, const float mass_b,
                                 const float distance) {
  const float inverted_distance = 1.0f / distance;
  return mass_a * inverted_distance * mass_b * inverted_distance;
}

} // namespace galaxy