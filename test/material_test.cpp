#include "pathtracer/object/material.hpp"

#include <gtest/gtest.h>
#include <cmath>

using namespace pathtracer;

TEST(Material, DiffuseScattersIntoSurfaceHemisphereUsingSuppliedGenerator)
{
    const Diffuse diffuse{Color{0.2f, 0.4f, 0.6f}};
    const Material& material = diffuse;
    const Ray incoming{Vec3{0, 0, 0}, Vec3{0, 0, -2}};
    const Vec3 normal{0, 0, 1};
    std::mt19937 generator{42};
    std::mt19937 replay{42};

    for (int i = 0; i < 32; ++i)
    {
        const auto scatter = material.scatter(incoming, normal, true, generator);
        const auto repeated = material.scatter(incoming, normal, true, replay);
        ASSERT_TRUE(scatter);
        ASSERT_TRUE(repeated);
        EXPECT_GT(Vec3::dot(scatter->direction, normal), 0.0f);
        EXPECT_NEAR(scatter->direction.length(), 1.0f, 1e-6f);
        EXPECT_FLOAT_EQ(scatter->direction.x, repeated->direction.x);
        EXPECT_FLOAT_EQ(scatter->direction.y, repeated->direction.y);
        EXPECT_FLOAT_EQ(scatter->direction.z, repeated->direction.z);
        EXPECT_FLOAT_EQ(scatter->attenuation.x, 0.2f);
        EXPECT_FLOAT_EQ(scatter->attenuation.y, 0.4f);
        EXPECT_FLOAT_EQ(scatter->attenuation.z, 0.6f);
    }
    EXPECT_EQ(generator, replay);
    EXPECT_NE(generator, (std::mt19937{42}));
}

TEST(Material, SmoothMetalReflectsWithoutConsumingRandomSamples)
{
    const Metal metal{Color{0.9f, 0.8f, 0.7f}};
    std::mt19937 generator{42};
    const auto scatter = metal.scatter(Ray{Vec3{}, Vec3{2, -2, 0}}, Vec3{0, 1, 0}, true, generator);

    ASSERT_TRUE(scatter);
    EXPECT_NEAR(scatter->direction.x, std::sqrt(0.5f), 1e-6f);
    EXPECT_NEAR(scatter->direction.y, std::sqrt(0.5f), 1e-6f);
    EXPECT_FLOAT_EQ(scatter->direction.z, 0.0f);
    EXPECT_FLOAT_EQ(scatter->attenuation.x, 0.9f);
    EXPECT_FLOAT_EQ(scatter->attenuation.y, 0.8f);
    EXPECT_FLOAT_EQ(scatter->attenuation.z, 0.7f);
    EXPECT_EQ(generator, (std::mt19937{42}));
}

TEST(Material, MetalAbsorbsReflectionsBelowSurface)
{
    const Metal metal;
    std::mt19937 generator{42};
    EXPECT_FALSE(metal.scatter(Ray{Vec3{}, Vec3{0, 1, 0}}, Vec3{0, 1, 0}, true, generator));
}

TEST(Material, RoughMetalPerturbsReflection)
{
    const Metal metal{Color{1, 1, 1}, 0.2f};
    std::mt19937 generator{42};
    const auto scatter = metal.scatter(Ray{Vec3{}, Vec3{0, 0, -1}}, Vec3{0, 0, 1}, true, generator);

    ASSERT_TRUE(scatter);
    EXPECT_GT(scatter->direction.z, 0.0f);
    EXPECT_GT(scatter->direction.x * scatter->direction.x + scatter->direction.y * scatter->direction.y, 0.0f);
    EXPECT_NEAR(scatter->direction.length(), 1.0f, 1e-6f);
}

TEST(Material, DielectricUsesEnteringStateForRefraction)
{
    const Dielectric glass{Color{1, 1, 1}, 1.5f};
    for (bool entering : {true, false})
    {
        std::mt19937 generator{42};
        const Vec3 normal = entering ? Vec3{0, 0, 1} : Vec3{0, 0, -1};
        const float z = std::sqrt(1.0f - 0.3f * 0.3f);
        const Ray incoming{Vec3{}, Vec3{0.3f, 0, entering ? -z : z}};
        const float refracted_x = entering ? 0.3f / 1.5f : 0.3f * 1.5f;
        int refractions = 0;

        for (int i = 0; i < 32; ++i)
        {
            const auto scatter = glass.scatter(incoming, normal, entering, generator);
            ASSERT_TRUE(scatter);
            EXPECT_NEAR(scatter->direction.length(), 1.0f, 1e-6f);
            EXPECT_FLOAT_EQ(scatter->attenuation.x, 1.0f);
            EXPECT_FLOAT_EQ(scatter->attenuation.y, 1.0f);
            EXPECT_FLOAT_EQ(scatter->attenuation.z, 1.0f);
            if (Vec3::dot(scatter->direction, normal) < 0.0f)
            {
                ++refractions;
                EXPECT_NEAR(scatter->direction.x, refracted_x, 1e-6f);
            }
            else
            {
                EXPECT_NEAR(scatter->direction.x, incoming.direction.x, 1e-6f);
                EXPECT_NEAR(scatter->direction.z, -incoming.direction.z, 1e-6f);
            }
        }
        EXPECT_GT(refractions, 0);
    }
}

TEST(Material, DielectricReflectsWhenExitingAboveCriticalAngle)
{
    const Dielectric glass;
    std::mt19937 generator{42};
    const Ray incoming{Vec3{}, Vec3{std::sqrt(0.75f), 0, 0.5f}};
    const auto scatter = glass.scatter(incoming, Vec3{0, 0, -1}, false, generator);

    ASSERT_TRUE(scatter);
    EXPECT_NEAR(scatter->direction.x, incoming.direction.x, 1e-6f);
    EXPECT_FLOAT_EQ(scatter->direction.y, 0.0f);
    EXPECT_NEAR(scatter->direction.z, -0.5f, 1e-6f);
}
