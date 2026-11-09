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

TEST(Vec3, DotProduct)
{
    EXPECT_FLOAT_EQ(Vec3::dot(Vec3{1, 2, 3}, Vec3{4, -5, 6}), 12.0f);
    EXPECT_FLOAT_EQ(Vec3::dot(Vec3{1, 0, 0}, Vec3{0, 1, 0}), 0.0f);
}

TEST(Vec3, CrossProductUsesRightHandRule)
{
    const Vec3 x{1, 0, 0};
    const Vec3 y{0, 1, 0};
    const Vec3 z{0, 0, 1};

    ExpectVecEq(Vec3::cross(x, y), 0, 0, 1);
    ExpectVecEq(Vec3::cross(y, z), 1, 0, 0);
    ExpectVecEq(Vec3::cross(z, x), 0, 1, 0);
    ExpectVecEq(Vec3::cross(y, x), 0, 0, -1);
}

TEST(Vec3, CrossProductHandlesNonUnitAndParallelVectors)
{
    const Vec3 a{1, 2, 3};
    const Vec3 b{4, -5, 6};
    const Vec3 perpendicular = Vec3::cross(a, b);

    ExpectVecEq(perpendicular, 27, 6, -13);
    EXPECT_FLOAT_EQ(Vec3::dot(perpendicular, a), 0.0f);
    EXPECT_FLOAT_EQ(Vec3::dot(perpendicular, b), 0.0f);
    ExpectVecEq(Vec3::cross(a, 2.0f * a), 0, 0, 0);
    ExpectVecEq(Vec3::cross(a, Vec3{0, 0, 0}), 0, 0, 0);
}

TEST(Vec3, Length)
{
    const Vec3 vector{3, 4, 12};
    EXPECT_FLOAT_EQ(vector.length_squared(), 169.0f);
    EXPECT_FLOAT_EQ(vector.length(), 13.0f);
}

TEST(Vec3, NormalizationPreservesDirectionAndProducesUnitLength)
{
    const Vec3 normal = Vec3{-3, 0, 4}.normalized();
    ExpectVecEq(normal, -0.6f, 0.0f, 0.8f);
    EXPECT_FLOAT_EQ(normal.length(), 1.0f);
}

TEST(Vec3, NormalizingZeroReturnsZero)
{
    ExpectVecEq(Vec3{0, 0, 0}.normalized(), 0.0f, 0.0f, 0.0f);
}

TEST(Vec3, ScalarDivision)
{
    ExpectVecEq(Vec3{2, -4, 6} / 2.0f, 1.0f, -2.0f, 3.0f);
}

TEST(Vec3, ReflectionReversesNormalComponentAndPreservesTangentialComponent)
{
    const Vec3 reflected = Vec3::reflect(Vec3{2, -3, 1}, Vec3{0, 1, 0});
    ExpectVecEq(reflected, 2, 3, 1);
}

TEST(Vec3, ReflectionHandlesAnObliqueSurfaceNormal)
{
    const Vec3 reflected = Vec3::reflect(Vec3{1, 0, 0}, Vec3{0.6f, 0.8f, 0});
    ExpectVecEq(reflected, 0.28f, -0.96f, 0);
}
