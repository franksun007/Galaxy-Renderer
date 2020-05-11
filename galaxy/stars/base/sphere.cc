
#include "sphere.h"

#include "galaxy/base/constants.h"
#include "galaxy/base/vec-type.h"

namespace galaxy {

Sphere::Sphere()
    : center(Vec3D(0, 0, 0)), radius(100), mass(10000),
      velocity(Vec3D(1, 1, 0)), self_rotation(Vec3D(0, 1, 0)) {}

Sphere::Sphere(const Vec3D center, const float radius, const float mass)
    : center(center), radius(radius), mass(mass), velocity(Vec3D(1, 1, 0)),
      self_rotation(Vec3D(0, 1, 0)) {}

Sphere::Sphere(const Vec3D center, const float radius, const float mass,
               const Vec3D velocity, const Vec3D self_rotation)
    : center(center), radius(radius), mass(mass), velocity(velocity),
      self_rotation(self_rotation) {}

float Sphere::SurfaceArea() const {
  return 4.0f * constants::kPi * radius * radius;
}

float Sphere::Volumn() const {
  return (4.0f / 3.0f) * constants::kPi * radius * radius * radius;
}

} // namespace galaxy
