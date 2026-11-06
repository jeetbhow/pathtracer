#include "scene.hpp"

#include <gtest/gtest.h>

TEST(Scene, RayColorBlendsSkyFromWhiteToBlue)
{
    const Vec3 origin{0, 0, 0};
    const Color down = ray_color(Ray{origin, Vec3{0, -2, 0}});
    const Color up = ray_color(Ray{origin, Vec3{0, 2, 0}});

    EXPECT_FLOAT_EQ(down.x, 1.0f);
    EXPECT_FLOAT_EQ(down.y, 1.0f);
    EXPECT_FLOAT_EQ(down.z, 1.0f);
    EXPECT_FLOAT_EQ(up.x, 0.5f);
    EXPECT_FLOAT_EQ(up.y, 0.7f);
    EXPECT_FLOAT_EQ(up.z, 1.0f);
}

// Pixels are sampled at their centers; the camera looks along -Z with +Y up.
// A non-square image also helps catch swapped row/column or width/height usage.
TEST(Camera, GenerateRayThroughImageCenterPointsForward)
{
    Image image{7, 3};
    Camera camera{image};

    const Ray ray = camera.generate_ray(1, 3);
    const Vec3 direction = ray.direction();

    EXPECT_FLOAT_EQ(ray.origin().x, 0.0f);
    EXPECT_FLOAT_EQ(ray.origin().y, 0.0f);
    EXPECT_FLOAT_EQ(ray.origin().z, 0.0f);
    EXPECT_NEAR(direction.x, 0.0f, 1e-6f);
    EXPECT_NEAR(direction.y, 0.0f, 1e-6f);
    EXPECT_LT(direction.z, 0.0f);
}

TEST(Camera, GenerateRayColumnsSpanLeftToRightSymmetrically)
{
    Image image{7, 3};
    Camera camera{image};

    const Vec3 left = camera.generate_ray(1, 0).direction();
    const Vec3 right = camera.generate_ray(1, 6).direction();

    EXPECT_LT(left.x, 0.0f);
    EXPECT_GT(right.x, 0.0f);
    EXPECT_FLOAT_EQ(left.x, -right.x);
    EXPECT_NEAR(left.y, 0.0f, 1e-6f);
    EXPECT_NEAR(right.y, 0.0f, 1e-6f);
    EXPECT_LT(left.z, 0.0f);
    EXPECT_FLOAT_EQ(left.z, right.z);
}

TEST(Camera, GenerateRayRowsSpanTopToBottomSymmetrically)
{
    Image image{7, 3};
    Camera camera{image};

    const Vec3 top = camera.generate_ray(0, 3).direction();
    const Vec3 bottom = camera.generate_ray(2, 3).direction();

    EXPECT_GT(top.y, 0.0f);
    EXPECT_LT(bottom.y, 0.0f);
    EXPECT_FLOAT_EQ(top.y, -bottom.y);
    EXPECT_NEAR(top.x, 0.0f, 1e-6f);
    EXPECT_NEAR(bottom.x, 0.0f, 1e-6f);
    EXPECT_LT(top.z, 0.0f);
    EXPECT_FLOAT_EQ(top.z, bottom.z);
}
