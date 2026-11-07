#include "pathtracer/camera/camera.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

using std::invalid_argument;
using std::mt19937;
using std::uniform_real_distribution;

using namespace pathtracer;

TEST(Camera, RejectsZeroDimensions)
{
    Image empty_width{0, 3};
    Image empty_height{7, 0};
    EXPECT_THROW((Camera{empty_width}), invalid_argument);
    EXPECT_THROW((Camera{empty_height}), invalid_argument);
}

TEST(Camera, StoresConfiguredSampleCount)
{
    Image image{7, 3};
    const Camera camera{image, 5};
    EXPECT_EQ(camera.samples, 5u);
}

TEST(Camera, SamplesWithinPixelUsingSuppliedGenerator)
{
    Image image{7, 3};
    const Camera camera{image, 16};
    mt19937 generator{42};
    mt19937 replay{42};

    for (unsigned int i = 0; i < camera.samples; ++i)
    {
        const Vec3 direction = camera.generate_ray(0, 0, generator).direction;
        const Vec3 repeated = camera.generate_ray(0, 0, replay).direction;
        EXPECT_GE(direction.x, -7.0f / 3.0f);
        EXPECT_LT(direction.x, -5.0f / 3.0f);
        EXPECT_GT(direction.y, 1.0f / 3.0f);
        EXPECT_LE(direction.y, 1.0f);
        EXPECT_FLOAT_EQ(direction.z, -1.0f);
        EXPECT_FLOAT_EQ(direction.x, repeated.x);
        EXPECT_FLOAT_EQ(direction.y, repeated.y);
    }
    EXPECT_EQ(generator, replay);
    EXPECT_NE(generator, (mt19937{42}));
}

TEST(Camera, TranslationPreservesRayDirections)
{
    Image image{7, 3};
    Camera camera{image};
    mt19937 generator{42};
    mt19937 replay{42};
    const Ray original = camera.generate_ray(0, 0, generator);

    camera.origin = Vec3{3, -2, 5};
    const Ray translated = camera.generate_ray(0, 0, replay);

    EXPECT_FLOAT_EQ(translated.origin.x, 3);
    EXPECT_FLOAT_EQ(translated.origin.y, -2);
    EXPECT_FLOAT_EQ(translated.origin.z, 5);
    EXPECT_FLOAT_EQ(translated.direction.x, original.direction.x);
    EXPECT_FLOAT_EQ(translated.direction.y, original.direction.y);
    EXPECT_FLOAT_EQ(translated.direction.z, original.direction.z);
}

// The camera looks along -Z with +Y up. A non-square image catches swapped axes.
TEST(Camera, GenerateRaySamplesCentralPixel)
{
    Image image{7, 3};
    const Camera camera{image};
    mt19937 generator{42};
    const Ray ray = camera.generate_ray(1, 3, generator);
    const Vec3 direction = ray.direction;

    EXPECT_FLOAT_EQ(ray.origin.x, 0.0f);
    EXPECT_FLOAT_EQ(ray.origin.y, 0.0f);
    EXPECT_FLOAT_EQ(ray.origin.z, 0.0f);
    EXPECT_GE(direction.x, -1.0f / 3.0f);
    EXPECT_LT(direction.x, 1.0f / 3.0f);
    EXPECT_GT(direction.y, -1.0f / 3.0f);
    EXPECT_LE(direction.y, 1.0f / 3.0f);
    EXPECT_FLOAT_EQ(direction.z, -1.0f);
}

TEST(Camera, GenerateRaySamplesPixelAtImageAspectRatio)
{
    Image image{7, 3};
    const Camera camera{image};
    mt19937 generator{42};
    mt19937 replay{42};
    uniform_real_distribution<float> distribution{0.0f, 1.0f};
    const float col_offset = distribution(replay);
    const float row_offset = distribution(replay);
    const Vec3 direction = camera.generate_ray(0, 0, generator).direction;

    EXPECT_NEAR(direction.x, (-7.0f + 2.0f * col_offset) / 3.0f, 1e-6f);
    EXPECT_NEAR(direction.y, 1.0f - 2.0f * row_offset / 3.0f, 1e-6f);
    EXPECT_FLOAT_EQ(direction.z, -1.0f);
}

TEST(Camera, GenerateRayColumnsSpanLeftToRight)
{
    Image image{7, 3};
    const Camera camera{image};
    mt19937 left_generator{42};
    mt19937 right_generator{42};
    const Vec3 left = camera.generate_ray(1, 0, left_generator).direction;
    const Vec3 right = camera.generate_ray(1, 6, right_generator).direction;

    EXPECT_LT(left.x, 0.0f);
    EXPECT_GT(right.x, 0.0f);
    EXPECT_NEAR(right.x - left.x, 4.0f, 1e-6f);
    EXPECT_FLOAT_EQ(left.y, right.y);
    EXPECT_FLOAT_EQ(left.z, -1.0f);
    EXPECT_FLOAT_EQ(left.z, right.z);
}

TEST(Camera, GenerateRayRowsSpanTopToBottom)
{
    Image image{7, 3};
    const Camera camera{image};
    mt19937 top_generator{42};
    mt19937 bottom_generator{42};
    const Vec3 top = camera.generate_ray(0, 3, top_generator).direction;
    const Vec3 bottom = camera.generate_ray(2, 3, bottom_generator).direction;

    EXPECT_GT(top.y, 0.0f);
    EXPECT_LT(bottom.y, 0.0f);
    EXPECT_NEAR(top.y - bottom.y, 4.0f / 3.0f, 1e-6f);
    EXPECT_FLOAT_EQ(top.x, bottom.x);
    EXPECT_FLOAT_EQ(top.z, -1.0f);
    EXPECT_FLOAT_EQ(top.z, bottom.z);
}
