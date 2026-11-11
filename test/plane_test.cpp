#include "pathtracer/geometry/plane.hpp"
#include "pathtracer/object/object.hpp"

#include <memory>
#include <gtest/gtest.h>
#include <stdexcept>

using namespace pathtracer;

TEST(Plane, PositionCanBeChangedThroughObjectInterface)
{
    Object plane{std::make_unique<Plane>(Vec3{0, -1, 0}, Vec3{0, 1, 0}), Diffuse{}};
    Object& object = plane;
    EXPECT_FLOAT_EQ(object.position().y, -1);
    object.position() = Vec3{0, -3, 0};

    const auto hit = object.hit(Ray{Vec3{0, 0, 0}, Vec3{0, -2, 0}});
    ASSERT_TRUE(hit);
    EXPECT_FLOAT_EQ(hit->t, 1.5f);
    EXPECT_FLOAT_EQ(hit->position.y, -3);
    EXPECT_FLOAT_EQ(hit->normal.y, 1);
}

TEST(Plane, ComputesUnitNormalIndependentOfPosition)
{
    const Object plane{std::make_unique<Plane>(Vec3{0, -1, 0}, Vec3{0, 0.6f, 0.8f}), Diffuse{}};
    const Object& object = plane;
    const Vec3 normal = object.normal_at(Vec3{5, 10, -3});
    EXPECT_FLOAT_EQ(normal.x, 0);
    EXPECT_FLOAT_EQ(normal.y, 0.6f);
    EXPECT_FLOAT_EQ(normal.z, 0.8f);
}

TEST(Plane, IntersectsNonUnitRayThroughObjectInterface)
{
    const Object plane{std::make_unique<Plane>(Vec3{0, -1, 0}, Vec3{0, 1, 0}), Diffuse{}};
    const Object& object = plane;
    const Ray ray{Vec3{3, 3, 5}, Vec3{0, -2, 0}};
    const auto hit = object.hit(ray);
    ASSERT_TRUE(hit);
    EXPECT_FLOAT_EQ(hit->t, 2);
    EXPECT_FLOAT_EQ(hit->position.x, 3);
    EXPECT_FLOAT_EQ(hit->position.y, -1);
    EXPECT_FLOAT_EQ(hit->position.z, 5);
    EXPECT_FLOAT_EQ(hit->normal.y, 1);
    EXPECT_TRUE(hit->entering);
    EXPECT_EQ(&hit->material, &plane.material);
    EXPECT_FLOAT_EQ(hit->incoming.direction.y, -2);
}

TEST(Plane, IntersectsTiltedPlane)
{
    const Object plane{std::make_unique<Plane>(Vec3{0, 1, 0}, normalize(Vec3{0, 1, 1})), Diffuse{}};
    const auto hit = plane.hit(Ray{Vec3{2, 3, 4}, Vec3{0, -1, -1}});
    ASSERT_TRUE(hit);
    EXPECT_NEAR(hit->t, 3, 1e-6f);
    EXPECT_NEAR(hit->position.y, 0, 1e-6f);
    EXPECT_NEAR(hit->position.z, 1, 1e-6f);
    EXPECT_NEAR(length(hit->normal), 1, 1e-6f);
}

TEST(Plane, FlipsShadingNormalForBackFaceHit)
{
    const Object plane{std::make_unique<Plane>(Vec3{0, -1, 0}, Vec3{0, 1, 0}), Diffuse{}};
    const auto hit = plane.hit(Ray{Vec3{0, -3, 0}, Vec3{0, 1, 0}});
    ASSERT_TRUE(hit);
    EXPECT_FLOAT_EQ(hit->t, 2);
    EXPECT_FLOAT_EQ(hit->normal.y, -1);
    EXPECT_FALSE(hit->entering);
    EXPECT_LT(dot(hit->incoming.direction, hit->normal), 0);
}

TEST(Plane, RejectsParallelCoplanarAndStationaryRays)
{
    const Object plane{std::make_unique<Plane>(Vec3{0, -1, 0}, Vec3{0, 1, 0}), Diffuse{}};
    EXPECT_FALSE(plane.hit(Ray{Vec3{0, 0, 0}, Vec3{1, 0, 0}}));
    EXPECT_FALSE(plane.hit(Ray{Vec3{0, -1, 0}, Vec3{1, 0, 0}}));
    EXPECT_FALSE(plane.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, 0}}));
}

TEST(Plane, RejectsHitsBehindOriginAndAtOrBelowMinimumDistance)
{
    const Object plane{std::make_unique<Plane>(Vec3{0, -1, 0}, Vec3{0, 1, 0}), Diffuse{}};
    EXPECT_FALSE(plane.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 1, 0}}));
    EXPECT_FALSE(plane.hit(Ray{Vec3{0, -1, 0}, Vec3{0, -1, 0}}));
    const Ray ray{Vec3{0, 0, 0}, Vec3{0, -1, 0}};
    EXPECT_TRUE(plane.hit(ray, 0.5f));
    EXPECT_FALSE(plane.hit(ray, 1.0f));
    EXPECT_FALSE(plane.hit(ray, 2.0f));
}

TEST(Plane, RejectsZeroNormal)
{
    EXPECT_THROW((Plane{Vec3{0, 0, 0}, Vec3{0, 0, 0}}), std::invalid_argument);
}
