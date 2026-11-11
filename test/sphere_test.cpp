#include "pathtracer/geometry/sphere.hpp"
#include "pathtracer/object/object.hpp"

#include <memory>
#include <gtest/gtest.h>

using namespace pathtracer;

TEST(Sphere, PositionCanBeChangedThroughObjectInterface)
{
    Object sphere{std::make_unique<Sphere>(Vec3{0, 0, -5}, 1), Diffuse{}};
    Object& object = sphere;
    EXPECT_FLOAT_EQ(object.position().z, -5);
    object.position() = Vec3{2, 0, -5};

    EXPECT_FALSE(object.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, -1}}));
    const auto hit = object.hit(Ray{Vec3{2, 0, 0}, Vec3{0, 0, -1}});
    ASSERT_TRUE(hit);
    EXPECT_FLOAT_EQ(hit->t, 4);
    EXPECT_FLOAT_EQ(hit->position.x, 2);
    EXPECT_FLOAT_EQ(hit->normal.z, 1);
}

TEST(Sphere, ComputesSurfaceNormal)
{
    const Object sphere{std::make_unique<Sphere>(Vec3{0, 0, -3}, 2.0f), Diffuse{}};
    const Object& object = sphere;
    const Vec3 normal = object.normal_at(Vec3{0, 2, -3});
    EXPECT_FLOAT_EQ(normal.x, 0.0f);
    EXPECT_FLOAT_EQ(normal.y, 1.0f);
    EXPECT_FLOAT_EQ(normal.z, 0.0f);
}

TEST(Sphere, ReturnsNearestHitForNonUnitRay)
{
    const Object sphere{std::make_unique<Sphere>(Vec3{0, 0, -5}, 1), Diffuse{}};
    const Object& object = sphere;
    const auto hit = object.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, -2}});
    ASSERT_TRUE(hit.has_value());
    EXPECT_EQ(&hit->material, &sphere.material);
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
    const Object sphere{std::make_unique<Sphere>(Vec3{0, 0, -5}, 1), Diffuse{}};
    const auto inside = sphere.hit(Ray{Vec3{0, 0, -5}, Vec3{1, 0, 0}});
    ASSERT_TRUE(inside.has_value());
    EXPECT_EQ(&inside->material, &sphere.material);
    EXPECT_FLOAT_EQ(inside->t, 1.0f);
    EXPECT_FLOAT_EQ(inside->position.x, 1.0f);
    EXPECT_FLOAT_EQ(inside->position.y, 0.0f);
    EXPECT_FLOAT_EQ(inside->position.z, -5.0f);
    EXPECT_FLOAT_EQ(inside->normal.x, -1.0f);
    EXPECT_FLOAT_EQ(inside->normal.y, 0.0f);
    EXPECT_FLOAT_EQ(inside->normal.z, 0.0f);

    const auto surface = sphere.hit(Ray{Vec3{0, 0, -4}, Vec3{0, 0, -1}});
    ASSERT_TRUE(surface.has_value());
    EXPECT_EQ(&surface->material, &sphere.material);
    EXPECT_FLOAT_EQ(surface->t, 2.0f);
    EXPECT_LE(dot(surface->incoming.direction, surface->normal), 0.0f);
    EXPECT_FALSE(sphere.hit(Ray{Vec3{0, 0, -4}, Vec3{0, 0, 1}}).has_value());
}

TEST(Sphere, ReturnsTangentHit)
{
    const Object sphere{std::make_unique<Sphere>(Vec3{0, 0, -5}, 1), Diffuse{}};
    const auto hit = sphere.hit(Ray{Vec3{1, 0, 0}, Vec3{0, 0, -1}});
    ASSERT_TRUE(hit.has_value());
    EXPECT_EQ(&hit->material, &sphere.material);
    EXPECT_FLOAT_EQ(hit->t, 5.0f);
    EXPECT_FLOAT_EQ(hit->position.x, 1.0f);
    EXPECT_FLOAT_EQ(hit->position.y, 0.0f);
    EXPECT_FLOAT_EQ(hit->position.z, -5.0f);
    EXPECT_FLOAT_EQ(hit->normal.x, 1.0f);
    EXPECT_FLOAT_EQ(hit->normal.y, 0.0f);
    EXPECT_FLOAT_EQ(hit->normal.z, 0.0f);
}

TEST(Sphere, SkipsRootsAtOrBelowMinimumDistance)
{
    const Object sphere{std::make_unique<Sphere>(Vec3{0, 0, -5}, 1), Diffuse{}};
    const Ray ray{Vec3{0, 0, 0}, Vec3{0, 0, -1}};

    const auto far_hit = sphere.hit(ray, 4.0f);
    ASSERT_TRUE(far_hit.has_value());
    EXPECT_FLOAT_EQ(far_hit->t, 6.0f);
    EXPECT_FALSE(sphere.hit(ray, 6.0f).has_value());
}

TEST(Sphere, ReturnsNoHitForMissBackwardOrStationaryRay)
{
    const Object sphere{std::make_unique<Sphere>(Vec3{0, 0, -5}, 1), Diffuse{}};
    EXPECT_FALSE(sphere.hit(Ray{Vec3{2, 0, 0}, Vec3{0, 0, -1}}).has_value());
    EXPECT_FALSE(sphere.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, 1}}).has_value());
    EXPECT_FALSE(sphere.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, 0}}).has_value());
}

TEST(Sphere, PreservesSmallRadiusAtLargeDistance)
{
    const Object sphere{std::make_unique<Sphere>(Vec3{0, 0, -10000}, 1), Diffuse{}};
    const auto hit = sphere.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, -1}});
    ASSERT_TRUE(hit.has_value());
    EXPECT_FLOAT_EQ(hit->t, 9999.0f);
    EXPECT_FLOAT_EQ(hit->position.z, -9999.0f);
    EXPECT_FLOAT_EQ(hit->normal.z, 1.0f);
    EXPECT_FLOAT_EQ(length(hit->normal), 1.0f);

    // Cancellation must not turn a nearby miss into a tangent hit, either.
    EXPECT_FALSE(sphere.hit(Ray{Vec3{2, 0, 0}, Vec3{0, 0, -1}}).has_value());
}

TEST(Sphere, InteriorReflectionStaysInTheShadingHemisphere)
{
    const Object sphere{std::make_unique<Sphere>(Vec3{0, 0, 0}, 2), Diffuse{}};
    const auto hit = sphere.hit(Ray{Vec3{0, 0, 0}, Vec3{0, 0, -1}});
    ASSERT_TRUE(hit.has_value());
    const Vec3 reflected = reflect(hit->incoming.direction, hit->normal);
    EXPECT_GT(dot(reflected, hit->normal), 0.0f);
    EXPECT_FLOAT_EQ(reflected.z, 1.0f);
}
