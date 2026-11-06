#include "pathtracer/camera.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

using namespace pathtracer;

TEST(Camera, RejectsZeroDimensions)
{
    EXPECT_THROW((Camera{0, 3}), std::invalid_argument);
    EXPECT_THROW((Camera{7, 0}), std::invalid_argument);
}

TEST(Camera, RejectsNonpositiveSampleCounts)
{
    EXPECT_THROW((Camera{7, 3, 0}), std::invalid_argument);
    EXPECT_THROW((Camera{7, 3, -1}), std::invalid_argument);
}

TEST(Camera, GeneratesConfiguredNumberOfSamplesPerPixel)
{
    const Camera camera{7, 3, 5};
    EXPECT_EQ(camera.samples_per_pixel(), 5);
    std::mt19937 generator{42};
    EXPECT_EQ(camera.generate_rays(1, 3, generator).size(), 5u);
}

TEST(Camera, SamplesWithinPixelUsingSuppliedGenerator)
{
    const Camera camera{7, 3, 16};
    std::mt19937 generator{42};
    std::mt19937 replay{42};

    const auto rays = camera.generate_rays(0, 0, generator);
    const auto repeated = camera.generate_rays(0, 0, replay);
    EXPECT_EQ(generator, replay);
    EXPECT_NE(generator, (std::mt19937{42}));

    for (std::size_t i = 0; i < rays.size(); ++i)
    {
        const Vec3 direction = rays[i].direction();
        EXPECT_GE(direction.x, -7.0f / 3.0f);
        EXPECT_LT(direction.x, -5.0f / 3.0f);
        EXPECT_GT(direction.y, 1.0f / 3.0f);
        EXPECT_LE(direction.y, 1.0f);
        EXPECT_FLOAT_EQ(direction.z, -1.0f);
        EXPECT_FLOAT_EQ(direction.x, repeated[i].direction().x);
        EXPECT_FLOAT_EQ(direction.y, repeated[i].direction().y);
    }
}

// Pixels are sampled at their centers; the camera looks along -Z with +Y up.
// A non-square image also helps catch swapped row/column or width/height usage.
TEST(Camera, GenerateRayThroughImageCenterPointsForward)
{
    Camera camera{7, 3};

    const Ray ray = camera.generate_ray(1, 3);
    const Vec3 direction = ray.direction();

    EXPECT_FLOAT_EQ(ray.origin().x, 0.0f);
    EXPECT_FLOAT_EQ(ray.origin().y, 0.0f);
    EXPECT_FLOAT_EQ(ray.origin().z, 0.0f);
    EXPECT_NEAR(direction.x, 0.0f, 1e-6f);
    EXPECT_NEAR(direction.y, 0.0f, 1e-6f);
    EXPECT_LT(direction.z, 0.0f);
}

TEST(Camera, GenerateRaySamplesPixelCenterAtImageAspectRatio)
{
    const Camera camera{7, 3};
    const Vec3 direction = camera.generate_ray(0, 0).direction();

    EXPECT_NEAR(direction.x, -2.0f, 1e-6f);
    EXPECT_NEAR(direction.y, 2.0f / 3.0f, 1e-6f);
    EXPECT_FLOAT_EQ(direction.z, -1.0f);
}

TEST(Camera, GenerateRayColumnsSpanLeftToRightSymmetrically)
{
    Camera camera{7, 3};

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
    Camera camera{7, 3};

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
