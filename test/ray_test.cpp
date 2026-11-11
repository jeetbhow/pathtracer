#include "pathtracer/geometry/ray.hpp"

#include <gtest/gtest.h>

using namespace pathtracer;

TEST(Ray, AtUsesOriginAndUnnormalizedDirection) {
    const Ray ray{Vec3{1, -2, 3}, Vec3{2, 4, -6}};

    const Vec3 origin = ray.at(0.0f);
    EXPECT_FLOAT_EQ(origin.x, 1.0f);
    EXPECT_FLOAT_EQ(origin.y, -2.0f);
    EXPECT_FLOAT_EQ(origin.z, 3.0f);

    const Vec3 forward = ray.at(0.5f);
    EXPECT_FLOAT_EQ(forward.x, 2.0f);
    EXPECT_FLOAT_EQ(forward.y, 0.0f);
    EXPECT_FLOAT_EQ(forward.z, 0.0f);

    const Vec3 backward = ray.at(-1.0f);
    EXPECT_FLOAT_EQ(backward.x, -1.0f);
    EXPECT_FLOAT_EQ(backward.y, -6.0f);
    EXPECT_FLOAT_EQ(backward.z, 9.0f);
}
