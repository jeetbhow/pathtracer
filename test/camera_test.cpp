#include "pathtracer/camera/camera.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

using std::invalid_argument;
using std::mt19937;
using std::uniform_real_distribution;

using namespace pathtracer;

TEST(Camera, InitializeRejectsZeroDimensions)
{
    Image empty_width{0, 3};
    Image empty_height{7, 0};
    Camera width_camera{empty_width};
    Camera height_camera{empty_height};
    EXPECT_THROW(width_camera.initialize(), invalid_argument);
    EXPECT_THROW(height_camera.initialize(), invalid_argument);
}

TEST(Camera, StoresConfiguredSampleCount)
{
    Image image{7, 3};
    Camera camera{image};
    camera.samples = 5;
    camera.initialize();
    EXPECT_EQ(camera.samples, 5u);
}

TEST(Camera, InitializeRebuildsViewportUsingCurrentFov)
{
    Image image{7, 3};
    Camera camera{image};
    camera.initialize();
    mt19937 wide_generator{42};
    mt19937 narrow_generator{42};
    mt19937 repeated_generator{42};
    const Vec3 wide = camera.generate_ray(0, 0, wide_generator).direction;

    camera.fov = 60;
    camera.initialize();
    const Vec3 narrow = camera.generate_ray(0, 0, narrow_generator).direction;

    // tan(30 degrees) / tan(45 degrees) = 1 / sqrt(3).
    EXPECT_NEAR(narrow.x, wide.x * 0.57735027f, 1e-6f);
    EXPECT_NEAR(narrow.y, wide.y * 0.57735027f, 1e-6f);
    EXPECT_FLOAT_EQ(narrow.z, wide.z);

    camera.initialize();
    const Vec3 repeated = camera.generate_ray(0, 0, repeated_generator).direction;
    EXPECT_FLOAT_EQ(repeated.x, narrow.x);
    EXPECT_FLOAT_EQ(repeated.y, narrow.y);
    EXPECT_FLOAT_EQ(repeated.z, narrow.z);
}

TEST(Camera, SamplesWithinPixelUsingSuppliedGenerator)
{
    Image image{7, 3};
    Camera camera{image};
    camera.samples = 16;
    camera.initialize();
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

TEST(Camera, TranslatingCameraAndTargetPreservesRayDirections)
{
    Image image{7, 3};
    Camera camera{image};
    camera.initialize();
    mt19937 generator{42};
    mt19937 replay{42};
    const Ray original = camera.generate_ray(0, 0, generator);

    camera.position = Vec3{3, -2, 5};
    camera.look_at = Vec3{3, -2, 4};
    camera.initialize();
    const Ray translated = camera.generate_ray(0, 0, replay);

    EXPECT_FLOAT_EQ(translated.origin.x, 3);
    EXPECT_FLOAT_EQ(translated.origin.y, -2);
    EXPECT_FLOAT_EQ(translated.origin.z, 5);
    EXPECT_FLOAT_EQ(translated.direction.x, original.direction.x);
    EXPECT_FLOAT_EQ(translated.direction.y, original.direction.y);
    EXPECT_FLOAT_EQ(translated.direction.z, original.direction.z);
}

TEST(Camera, GenerateRayAimsAtWorldSpaceTarget)
{
    Image image{1, 1};
    Camera camera{image};
    camera.look_at = Vec3{3, 1, 1};
    camera.position = Vec3{3, 4, 5};
    camera.initialize();
    mt19937 generator{42};
    mt19937 replay{42};
    uniform_real_distribution<float> distribution{0.0f, 1.0f};
    const float horizontal = 2.0f * distribution(replay) - 1.0f;
    const float vertical = 2.0f * distribution(replay) - 1.0f;
    const Ray ray = camera.generate_ray(0, 0, generator);

    // Forward is (0, -0.6, -0.8), right is +X, down is (0, -0.8, 0.6).
    EXPECT_FLOAT_EQ(ray.origin.x, 3);
    EXPECT_FLOAT_EQ(ray.origin.y, 4);
    EXPECT_FLOAT_EQ(ray.origin.z, 5);
    EXPECT_NEAR(ray.direction.x, horizontal, 1e-6f);
    EXPECT_NEAR(ray.direction.y, -0.6f - 0.8f * vertical, 1e-6f);
    EXPECT_NEAR(ray.direction.z, -0.8f + 0.6f * vertical, 1e-6f);

    // Reinitialize from the new position with the same world-space target.
    camera.position = Vec3{3, 1, 6};
    camera.initialize();
    mt19937 moved_generator{42};
    const Ray moved = camera.generate_ray(0, 0, moved_generator);
    EXPECT_NEAR(moved.direction.x, horizontal, 1e-6f);
    EXPECT_NEAR(moved.direction.y, -vertical, 1e-6f);
    EXPECT_FLOAT_EQ(moved.direction.z, -1);
}

TEST(Camera, InitializeRejectsTargetAtCameraPosition)
{
    Image image{1, 1};
    Camera camera{image};
    camera.position = Vec3{3, 4, 5};
    camera.look_at = Vec3{3, 4, 5};
    EXPECT_THROW(camera.initialize(), invalid_argument);
}

// The camera looks along -Z with +Y up. A non-square image catches swapped axes.
TEST(Camera, GenerateRaySamplesCentralPixel)
{
    Image image{7, 3};
    Camera camera{image};
    camera.initialize();
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
    Camera camera{image};
    camera.initialize();
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
    Camera camera{image};
    camera.initialize();
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
    Camera camera{image};
    camera.initialize();
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
