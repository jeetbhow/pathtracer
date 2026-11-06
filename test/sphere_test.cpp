#include "pathtracer/geometry/sphere.hpp"

#include <gtest/gtest.h>

using namespace pathtracer;

TEST(Sphere, ComputesSurfaceNormal)
{
    const Sphere sphere{Vec3{0, 0, -3}, 2.0f};
    const Vec3 normal = sphere.normal(Vec3{0, 2, -3});
    EXPECT_FLOAT_EQ(normal.x, 0.0f);
    EXPECT_FLOAT_EQ(normal.y, 1.0f);
    EXPECT_FLOAT_EQ(normal.z, 0.0f);
}

TEST(Sphere, ReturnsNearestHitForNonUnitRay)
{
    const Sphere sphere{Vec3{0, 0, -5}, 1};
    const auto hit = sphere.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, -2}});
    ASSERT_TRUE(hit.has_value());
    EXPECT_FLOAT_EQ(hit->t, 2.0f);
    EXPECT_FLOAT_EQ(hit->position.x, 0.0f);
    EXPECT_FLOAT_EQ(hit->position.y, 0.0f);
    EXPECT_FLOAT_EQ(hit->position.z, -4.0f);
    EXPECT_FLOAT_EQ(hit->normal.x, 0.0f);
    EXPECT_FLOAT_EQ(hit->normal.y, 0.0f);
    EXPECT_FLOAT_EQ(hit->normal.z, 1.0f);
}

TEST(Sphere, ReturnsExitHitWhenRayStartsInsideOrOnSurface)
{
    const Sphere sphere{Vec3{0, 0, -5}, 1};
    const auto inside = sphere.hit(Ray{Vec3{0, 0, -5}, Vec3{1, 0, 0}});
    ASSERT_TRUE(inside.has_value());
    EXPECT_FLOAT_EQ(inside->t, 1.0f);
    EXPECT_FLOAT_EQ(inside->position.x, 1.0f);
    EXPECT_FLOAT_EQ(inside->position.y, 0.0f);
    EXPECT_FLOAT_EQ(inside->position.z, -5.0f);
    EXPECT_FLOAT_EQ(inside->normal.x, 1.0f);
    EXPECT_FLOAT_EQ(inside->normal.y, 0.0f);
    EXPECT_FLOAT_EQ(inside->normal.z, 0.0f);

    const auto surface = sphere.hit(Ray{Vec3{0, 0, -4}, Vec3{0, 0, -1}});
    ASSERT_TRUE(surface.has_value());
    EXPECT_FLOAT_EQ(surface->t, 2.0f);
    EXPECT_FALSE(sphere.hit(Ray{Vec3{0, 0, -4}, Vec3{0, 0, 1}}).has_value());
}

TEST(Sphere, ReturnsTangentHit)
{
    const Sphere sphere{Vec3{0, 0, -5}, 1};
    const auto hit = sphere.hit(Ray{Vec3{1, 0, 0}, Vec3{0, 0, -1}});
    ASSERT_TRUE(hit.has_value());
    EXPECT_FLOAT_EQ(hit->t, 5.0f);
    EXPECT_FLOAT_EQ(hit->position.x, 1.0f);
    EXPECT_FLOAT_EQ(hit->position.y, 0.0f);
    EXPECT_FLOAT_EQ(hit->position.z, -5.0f);
    EXPECT_FLOAT_EQ(hit->normal.x, 1.0f);
    EXPECT_FLOAT_EQ(hit->normal.y, 0.0f);
    EXPECT_FLOAT_EQ(hit->normal.z, 0.0f);
}

TEST(Sphere, ReturnsNoHitForMissBackwardOrStationaryRay)
{
    const Sphere sphere{Vec3{0, 0, -5}, 1};
    EXPECT_FALSE(sphere.hit(Ray{Vec3{2, 0, 0}, Vec3{0, 0, -1}}).has_value());
    EXPECT_FALSE(sphere.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, 1}}).has_value());
    EXPECT_FALSE(sphere.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, 0}}).has_value());
}
