#include "vec-type.h"

#include "gtest/gtest.h"

namespace galaxy {
TEST(TestVecType, test_vec2d) {
  Vec2D a(99, 22);
  Vec2D b(23, 45);
  ASSERT_FALSE(a == b);
  ASSERT_EQ(a.x, 99.f);
  ASSERT_EQ(a.y, 22.f);

  Vec2D c(99, 22);
  ASSERT_TRUE(a == c);

  Vec2D d(99, 11);
  ASSERT_FALSE(a == d);

  Vec2D e(22, 22);
  ASSERT_FALSE(a == e);

  Vec2D aa = a;
  ASSERT_TRUE(aa == a);

  Vec2D m;
  Vec2D n = m;
  ASSERT_TRUE(n == m);
  ASSERT_EQ(m.x, 0.0);
  ASSERT_EQ(m.y, 0.0);
}

TEST(TestVecType, test_vec2d_to_string) {
  Vec2D a(99, 22);
  ASSERT_EQ(a.to_string(), "(99.000, 22.000)");
  Vec2D b(23, 45);
  ASSERT_EQ(b.to_string(), "(23.000, 45.000)");
  Vec2D d(99, 11);
  ASSERT_EQ(d.to_string(), "(99.000, 11.000)");
  Vec2D e(22, 22);
  ASSERT_EQ(e.to_string(), "(22.000, 22.000)");
}

TEST(TestVecType, test_vec3d) {
  Vec3D a(99, 22, 11);
  Vec3D b(23, 45, 93);
  ASSERT_FALSE(a == b);
  ASSERT_EQ(a.x, 99.0);
  ASSERT_EQ(a.y, 22.0);
  ASSERT_EQ(a.z, 11.0);

  Vec3D c(99, 22, 11);
  ASSERT_TRUE(a == c);

  Vec3D d(99, 11, 22);
  ASSERT_FALSE(a == d);

  Vec3D e(22, 22, 11);
  ASSERT_FALSE(a == e);

  Vec3D f(99, 22, 13);
  ASSERT_FALSE(a == e);

  Vec3D aa = a;
  ASSERT_TRUE(aa == a);

Vec3D m;
  Vec3D n = m;
  ASSERT_TRUE(n == m);
  ASSERT_EQ(m.x, 0.0);
  ASSERT_EQ(m.y, 0.0);
  ASSERT_EQ(m.z, 0.0);
}

TEST(TestVecType, test_vec3d_to_string) {
  Vec3D a(99, 22, 11);
  ASSERT_EQ(a.to_string(), "(99.000, 22.000, 11.000)");
  Vec3D b(23, 45, 93);
  ASSERT_EQ(b.to_string(), "(23.000, 45.000, 93.000)");
  Vec3D c(99, 22, 11);
  ASSERT_EQ(c.to_string(), "(99.000, 22.000, 11.000)");
  Vec3D d(99, 11, 22);
  ASSERT_EQ(d.to_string(), "(99.000, 11.000, 22.000)");
  Vec3D e(22, 22, 11);
  ASSERT_EQ(e.to_string(), "(22.000, 22.000, 11.000)");
  Vec3D f(99, 22, 13);
  ASSERT_EQ(f.to_string(), "(99.000, 22.000, 13.000)");
}

TEST(TestVecType, test_vec3d_mag_wrt_0) {
  Vec3D a(99, 22, 11);
  ASSERT_NEAR(a.MagnitudeWRT0(), 102.00980345045274, 1e-6);
}

TEST(TestVecType, test_vec3d_normalization) {
  Vec3D a(99, 22, 11);
  a.NormalizedByMagnitudeWRT0();

  ASSERT_NEAR(a.x, 99.0 / 102.00980345045274, 1e-6);
  ASSERT_NEAR(a.y, 22.0 / 102.00980345045274, 1e-6);
  ASSERT_NEAR(a.z, 11.0 / 102.00980345045274, 1e-6);
}

} // namespace galaxy