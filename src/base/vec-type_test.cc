#include "vec-type.h"

#include "gtest/gtest.h"

namespace galaxy {
TEST(TestVecType, test_vec2d) {
    Vec2D a(99, 22);
    Vec2D b(23, 45); 
    ASSERT_FALSE(a == b);

    Vec2D c(99, 22);
    ASSERT_TRUE(a == c);

    Vec2D d(99, 11);
    ASSERT_FALSE(a == d);

    Vec2D e(22, 22);
    ASSERT_FALSE(a == e);

    Vec2D aa = a;
    ASSERT_TRUE(aa == a);
}

TEST(TestVecType, test_vec3d) {
    Vec3D a(99, 22, 11);
    Vec3D b(23, 45, 93); 
    ASSERT_FALSE(a == b);

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
}
}  // namespace galaxy