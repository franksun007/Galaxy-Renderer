#include "gravitational-force.h"

#include "gtest/gtest.h"

namespace galaxy {

// TODO(Frank): More test cases

TEST(GravitationalForceTest, test_scaled_gforce) {
    const float mass_a = 19.0;
    const float mass_b = 21.0;
    const float distance = 14;
    // Result
    // Reference: 0.000000000135865
    const double reference = 0.000000000135865;

    const float scale = 10000;

    ASSERT_NEAR(
        ScaledGravitationalForce(mass_a, mass_b, distance, scale), 
        static_cast<float>(reference * scale),
        1e-7
    );
}

TEST(GravitationalForceTest, test_relative_gforce) {
    // Using double to ensure higher accuracy
    const double mass_a = 19.0;
    const double mass_b = 21.0;
    const double distance = 14;

    ASSERT_FLOAT_EQ(
        RelativeGravitationalForce(mass_a, mass_b, distance), 
        mass_a * mass_b / distance / distance
    );
}

}  // namespace galaxy