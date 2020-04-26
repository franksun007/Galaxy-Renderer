#ifndef __GALAXY_RENDERER_VECTOR_H__
#define __GALAXY_RENDERER_VECTOR_H__

#include <string>

struct Vec2D {
public:
  Vec2D(const float x_ = 0, const float y_ = 0) : x(x_), y(y_) {}
  float x;
  float y;

  std::string to_string() {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }

  // TODO(Frank): Same Center is OK??
  bool operator==(const Vec2D &other) { return x == other.x && y == other.y; }
};

struct Vec3D {
public:
  Vec3D(const float x_ = 0, const float y_ = 0, const float z_ = 0)
      : x(x_), y(y_), z(z_) {}
  float x;
  float y;
  float z;

  std::string to_string() {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z) + ")";
  }
};

#endif // __GALAXY_RENDERER_VECTOR_H__
