#include "geometry.hpp"

#include <gtest/gtest.h>

static void ExpectVecEq(const Vec3& v, float x, float y, float z)
{
    EXPECT_FLOAT_EQ(v.x, x);
    EXPECT_FLOAT_EQ(v.y, y);
    EXPECT_FLOAT_EQ(v.z, z);
}

TEST(Vec3, AggregateInitialization)
{
    Vec3 v{1.0f, 2.0f, 3.0f};
    ExpectVecEq(v, 1.0f, 2.0f, 3.0f);
}

TEST(Vec3, Addition)
{
    ExpectVecEq(Vec3{1, 2, 3} + Vec3{4, 5, 6}, 5, 7, 9);
}

TEST(Vec3, AdditionWithZeroIsIdentity)
{
    ExpectVecEq(Vec3{1, -2, 3} + Vec3{0, 0, 0}, 1, -2, 3);
}

TEST(Vec3, Subtraction)
{
    ExpectVecEq(Vec3{4, 5, 6} - Vec3{1, 2, 3}, 3, 3, 3);
}

TEST(Vec3, SubtractingSelfGivesZero)
{
    Vec3 v{1.5f, -2.5f, 3.5f};
    ExpectVecEq(v - v, 0, 0, 0);
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

TEST(Vec3, OperatorsDoNotMutateOperands)
{
    const Vec3 a{1, 2, 3};
    const Vec3 b{4, 5, 6};
    (void)(a + b);
    (void)(a - b);
    (void)(a * 3.0f);
    ExpectVecEq(a, 1, 2, 3);
    ExpectVecEq(b, 4, 5, 6);
}

TEST(Vec3, ChainedExpressions)
{
    ExpectVecEq((Vec3{1, 1, 1} + Vec3{2, 2, 2}) * 2.0f - Vec3{1, 1, 1}, 5, 5, 5);
}

TEST(Sphere, ReturnsNearestHitForNonUnitRay)
{
    const Sphere sphere{Vec3{0, 0, -5}, 1};
    const auto hit = sphere.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, -2}});
    ASSERT_TRUE(hit.has_value());
    EXPECT_FLOAT_EQ(*hit, 2.0f);
}

TEST(Sphere, ReturnsExitHitWhenRayStartsInsideOrOnSurface)
{
    const Sphere sphere{Vec3{0, 0, -5}, 1};
    const auto inside = sphere.hit(Ray{Vec3{0, 0, -5}, Vec3{1, 0, 0}});
    ASSERT_TRUE(inside.has_value());
    EXPECT_FLOAT_EQ(*inside, 1.0f);

    const auto surface = sphere.hit(Ray{Vec3{0, 0, -4}, Vec3{0, 0, -1}});
    ASSERT_TRUE(surface.has_value());
    EXPECT_FLOAT_EQ(*surface, 2.0f);
    EXPECT_FALSE(sphere.hit(Ray{Vec3{0, 0, -4}, Vec3{0, 0, 1}}).has_value());
}

TEST(Sphere, ReturnsTangentHit)
{
    const Sphere sphere{Vec3{0, 0, -5}, 1};
    const auto hit = sphere.hit(Ray{Vec3{1, 0, 0}, Vec3{0, 0, -1}});
    ASSERT_TRUE(hit.has_value());
    EXPECT_FLOAT_EQ(*hit, 5.0f);
}

TEST(Sphere, ReturnsNoHitForMissBackwardOrStationaryRay)
{
    const Sphere sphere{Vec3{0, 0, -5}, 1};
    EXPECT_FALSE(sphere.hit(Ray{Vec3{2, 0, 0}, Vec3{0, 0, -1}}).has_value());
    EXPECT_FALSE(sphere.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, 1}}).has_value());
    EXPECT_FALSE(sphere.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, 0}}).has_value());
}
