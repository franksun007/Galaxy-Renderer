#ifndef __GALAXY_STARTS_SPHERE_H__
#define __GALAXY_STARTS_SPHERE_H__

#include "galaxy/base/types.h"
#include "galaxy/base/vec-type.h"

namespace galaxy {

struct Sphere {
public:
  Sphere();
  Sphere(const Vec3D &center, const float radius, const float mass);
  Sphere(const Vec3D &center, const float radius, const float mass,
         const Vec3D &velocity, const Vec3D &self_rotation);

  Vec3D center;
  // in m
  float radius;
  // in kg
  float mass;

  // TODO(Frank): Whether to include those fields or not
  Vec3D velocity;
  Vec3D self_rotation;

  float SurfaceArea() const;
  float Volumn() const;
  float Density() const { return mass / Volumn(); }
};

} // namespace galaxy

#endif // __GALAXY_STARTS_SPHERE_H__