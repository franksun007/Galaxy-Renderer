#ifndef __GALAXY_BASE_VEC_TYPE_H__
#define __GALAXY_BASE_VEC_TYPE_H__

#include "types.h"
#include "logging.h"

#include <cmath>

namespace galaxy {

struct Vec2D {
 public:
  Vec2D(const float x = 0.f, const float y = 0.f);
  
  Vec2D(const Vec2D &v) { x = v.x; y = v.y; }

  // Public field for easy access
  float x;
  float y;

  const string to_string() const;
  void operator=(const Vec2D &other);
  bool operator==(const Vec2D &other) const;
};

struct Vec3D {
 public:
  Vec3D(const float x = 0.f, const float y = 0.f, const float z = 0.f);

  Vec3D(const Vec3D &v) { x = v.x; y = v.y; z = v.z; }

  // Public field for easy access
  float x;
  float y;
  float z;

  void NormalizedByMagnitudeWRT0();
  float MagnitudeWRT0() const;

  const string to_string() const;
  void operator=(const Vec3D &other);
  bool operator==(const Vec3D &other) const;
};

struct Force {
 public:
  Force(): point(Vec3D(0, 0, 0)), direction(Vec3D(0, 0, 0)), magnitude(0) {}
  Force(const Vec3D &point, const Vec3D& direction, const float magnitude):  
  point(point), direction(direction), magnitude(magnitude) {
    // TODO(Frank): Find a better way to check if the vector is normalized.
    CHECK_GE(direction.x, -1);
    CHECK_LE(direction.x, 1);
    CHECK_GE(direction.y, -1);
    CHECK_LE(direction.y, 1);
    CHECK_GE(direction.z, -1);
    CHECK_LE(direction.z, 1);
  }

  // point where this force is being applied.
  Vec3D point;
  // the direction of the force w.r.t (0, 0, 0)
  Vec3D direction;
  float magnitude;

  const string to_string() const;
  void operator=(const Force &other);
  bool operator==(const Force &other);
  Force operator+(const Force& other);
};

} // namespace galaxy

#endif // __GALAXY_BASE_VEC_TYPE_H__
