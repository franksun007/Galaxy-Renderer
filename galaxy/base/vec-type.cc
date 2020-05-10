
#include "vec-type.h"

#include "absl/strings/str_format.h"

#include "types.h"

namespace galaxy {
namespace {
constexpr float kEpsilon = 1e-7;
} // namespace

Vec2D::Vec2D(const float x_ = 0, const float y_ = 0) : x(x_), y(y_) {}

const string Vec2D::to_string() {
  return absl::StrFormat("(%.3f, %.3f)", x, y);
}

void Vec2D::operator=(const Vec2D &other) {
  x = other.x;
  y = other.y;
}

bool Vec2D::operator==(const Vec2D &other) {
  return std::abs(x - other.x) < kEpsilon && std::abs(y - other.y) < kEpsilon;
}

Vec3D::Vec3D(const float x_ = 0, const float y_ = 0, const float z_ = 0)
    : x(x_), y(y_), z(z_) {}

const string Vec3D::to_string() {
  return absl::StrFormat("(%.3f, %.3f, %.3f)", x, y, z);
}

void Vec3D::operator=(const Vec3D &other) {
  x = other.x;
  y = other.y;
  z = other.z;
}

bool Vec3D::operator==(const Vec3D &other) {
  return std::abs(x - other.x) < kEpsilon && std::abs(y - other.y) < kEpsilon &&
         std::abs(z - other.z) < kEpsilon;
}

} // namespace galaxy
