
#include "sphere.h"

#include "gtest/gtest.h"

#include "galaxy/base/vec-type.h"

namespace galaxy {

TEST(TestSphere, test_constructor) {
  const Vec3D default_center = Vec3D(0, 0, 0);
  const float default_radius = 100;
  const float default_mass = 10000;
  const Vec3D default_velocity = Vec3D(1, 1, 0);
  const Vec3D default_self_rotation = Vec3D(0, 1, 0);

  Sphere a;
  Sphere b(default_center, default_radius, default_mass);
  Sphere c(default_center, default_radius, default_mass, default_velocity,
           default_self_rotation);

  ASSERT_EQ(a.center, default_center);
  ASSERT_EQ(b.center, default_center);
  ASSERT_EQ(c.center, default_center);
  ASSERT_EQ(a.radius, default_radius);
  ASSERT_EQ(b.radius, default_radius);
  ASSERT_EQ(c.radius, default_radius);
  ASSERT_EQ(a.mass, default_mass);
  ASSERT_EQ(b.mass, default_mass);
  ASSERT_EQ(c.mass, default_mass);
  ASSERT_EQ(a.velocity, default_velocity);
  ASSERT_EQ(b.velocity, default_velocity);
  ASSERT_EQ(c.velocity, default_velocity);
  ASSERT_EQ(a.self_rotation, default_self_rotation);
  ASSERT_EQ(b.self_rotation, default_self_rotation);
  ASSERT_EQ(c.self_rotation, default_self_rotation);
}

TEST(TestSphere, test_properties) {
  const float default_mass = 10000;
  Sphere a;
  ASSERT_NEAR(a.SurfaceArea(), 125663.70616, 1e-1);
  ASSERT_NEAR(a.Volumn(), 4188790.20533, 1e-1);
  ASSERT_NEAR(a.Density(), 10000 / 4188790.20533, 1e-1);
}

} // namespace galaxy
