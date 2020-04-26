#ifndef __GALAXY_BASE_VEC_TYPE_H__
#define __GALAXY_BASE_VEC_TYPE_H__

#include <string>

namespace galaxy {
namespace {
constexpr float kEpsilon = 1e-7;
}  // namespace

struct Vec2D {
public:
  Vec2D(const float x_ = 0, const float y_ = 0) : x(x_), y(y_) {}
  float x;
  float y;

  const std::string to_string() {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }

  void operator=(const Vec2D& other) {
    x = other.x;
    y = other.y;
  }

  bool operator==(const Vec2D &other) {
    return std::abs(x - other.x) < kEpsilon && std::abs(y - other.y) < kEpsilon;
  }
};

struct Vec3D {
public:
  Vec3D(const float x_ = 0, const float y_ = 0, const float z_ = 0)
      : x(x_), y(y_), z(z_) {}
  float x;
  float y;
  float z;

  const std::string to_string() {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z) + ")";
  }

  void operator=(const Vec3D& other) {
    x = other.x;
    y = other.y;
    z = other.z;
  }

  bool operator==(const Vec3D &other) {
    return std::abs(x - other.x) < kEpsilon &&
           std::abs(y - other.y) < kEpsilon && 
           std::abs(z - other.z) < kEpsilon;
  }
};

} // namespace galaxy

#endif // __GALAXY_BASE_VEC_TYPE_H__
