#ifndef __GALAXY_BASE_VEC_TYPE_H__
#define __GALAXY_BASE_VEC_TYPE_H__

#include "types.h"

namespace galaxy {

struct Vec2D {
public:
  Vec2D(const float, const float);

  // Public field for easy access
  float x;
  float y;

  const string to_string();
  void operator=(const Vec2D &other);
  bool operator==(const Vec2D &other);
};

struct Vec3D {
public:
  Vec3D(const float, const float, const float);
  // Public field for easy access
  float x;
  float y;
  float z;

  const string to_string();
  void operator=(const Vec3D &other);
  bool operator==(const Vec3D &other);
};

} // namespace galaxy

#endif // __GALAXY_BASE_VEC_TYPE_H__
