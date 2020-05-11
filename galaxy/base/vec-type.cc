
#include "vec-type.h"

#include "absl/strings/str_format.h"

#include "logging.h"
#include "types.h"

namespace galaxy {
namespace {
constexpr float kEpsilon = 1e-7;
} // namespace

Vec2D::Vec2D(const float x, const float y) : x(x), y(y) {}

const string Vec2D::to_string() const {
  return absl::StrFormat("(%.3f, %.3f)", x, y);
}

void Vec2D::operator=(const Vec2D &other) {
  x = other.x;
  y = other.y;
}

bool Vec2D::operator==(const Vec2D &other) const {
  return std::abs(x - other.x) < kEpsilon && std::abs(y - other.y) < kEpsilon;
}

Vec3D::Vec3D(const float x, const float y, const float z)
    : x(x), y(y), z(z) {}

const string Vec3D::to_string() const {
  return absl::StrFormat("(%.3f, %.3f, %.3f)", x, y, z);
}

void Vec3D::operator=(const Vec3D &other) {
  x = other.x;
  y = other.y;
  z = other.z;
}

bool Vec3D::operator==(const Vec3D &other) const {
  return std::abs(x - other.x) < kEpsilon && std::abs(y - other.y) < kEpsilon &&
         std::abs(z - other.z) < kEpsilon;
}

void Vec3D::NormalizedByMagnitudeWRT0() {
  const float recip_mag = 1.0f / MagnitudeWRT0();
  x *= recip_mag;
  y *= recip_mag;
  z *= recip_mag;
}

float Vec3D::MagnitudeWRT0() const {
  return std::sqrt(x * x + y * y + z * z);
}

const string Force::to_string() const {
  return absl::StrFormat("[P: %s, D: %s, M: %.3f]", point.to_string(), direction.to_string(), magnitude);
}
void Force::operator=(const Force &other) {
  point = other.point;
  direction = other.direction;
  magnitude = other.magnitude;
}

bool Force::operator==(const Force &other) {
  return magnitude == other.magnitude && point == other.point && direction == other.direction; 
}

Force Force::operator+(const Force& other) {
  CHECK(point == other.point) << "Force addition currently is only valid for forces applied on the same point.";
  
  Vec3D new_direction;
  new_direction.x = direction.x * magnitude + other.direction.x * other.magnitude;
  new_direction.y = direction.y * magnitude + other.direction.y * other.magnitude;
  new_direction.z = direction.z * magnitude + other.direction.z * other.magnitude;
  
  const float new_mag = new_direction.MagnitudeWRT0();
  new_direction.NormalizedByMagnitudeWRT0();
  return Force(point, new_direction, new_mag);
}

} // namespace galaxy
