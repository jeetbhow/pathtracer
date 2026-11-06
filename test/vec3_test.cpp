#include "pathtracer/geometry/vec3.hpp"

#include <gtest/gtest.h>

using namespace pathtracer;

static void ExpectVecEq(const Vec3& v, float x, float y, float z)
{
    EXPECT_FLOAT_EQ(v.x, x);
    EXPECT_FLOAT_EQ(v.y, y);
    EXPECT_FLOAT_EQ(v.z, z);
}

TEST(Vec3, Addition)
{
    ExpectVecEq(Vec3{1, 2, 3} + Vec3{4, 5, 6}, 5, 7, 9);
}

TEST(Vec3, Subtraction)
{
    ExpectVecEq(Vec3{4, 5, 6} - Vec3{1, 2, 3}, 3, 3, 3);
}

TEST(Vec3, ScalarMultiplication)
{
    ExpectVecEq(Vec3{1, 2, 3} * 2.0f, 2, 4, 6);
}

TEST(Vec3, ScalarMultiplicationByZeroAndNegative)
{
    ExpectVecEq(Vec3{1, 2, 3} * 0.0f, 0, 0, 0);
    ExpectVecEq(Vec3{1, 2, 3} * -1.0f, -1, -2, -3);
}
