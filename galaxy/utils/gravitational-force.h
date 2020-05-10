#ifndef __GALAXY_UTILS_GRAVITATIONAL_FORCE_H__
#define __GALAXY_UTILS_GRAVITATIONAL_FORCE_H__

namespace galaxy {
constexpr float kScaleFactor = 1000;

// Provide the optional to scale up the real gravitational force for small
// mass elements.
// Calculate the Universial Graviational Force with scale:
//   F = (G * mass_a * mass_b / distance^2) * scale
float ScaledGravitationalForce(const float mass_a, const float mass_b,
                               const float distance,
                               const float scale = kScaleFactor);

// Relevant gravitation force for comparison only.
// Calculate the following:
//   F' = mass_a * mass_b / distance^2
float RelativeGravitationalForce(const float mass_a, const float mass_b,
                                 const float distance);


} // namespace galaxy

#endif // __GALAXY_UTILS_GRAVITATIONAL_FORCE_H__
