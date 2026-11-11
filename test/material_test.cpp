#include "pathtracer/object/material.hpp"
#include "pathtracer/object/object.hpp"

#include <gtest/gtest.h>
#include <cmath>

using std::mt19937;
using std::sqrt;

using namespace pathtracer;

TEST(Material, DiffuseScattersIntoSurfaceHemisphereUsingSuppliedGenerator) {
    const Material material = Diffuse{Color{0.2f, 0.4f, 0.6f}};
    const Ray incoming{Vec3{0, 0, 0}, Vec3{0, 0, -2}};
    const Vec3 normal{0, 0, 1};
    const ShadingContext hit{1.0f, incoming.at(1.0f), normal, material, incoming, true};
    mt19937 generator{42};
    mt19937 replay{42};

    for (int i = 0; i < 32; ++i) {
        const auto scatter = pathtracer::scatter(hit, generator);
        const auto repeated = pathtracer::scatter(hit, replay);
        ASSERT_TRUE(scatter);
        ASSERT_TRUE(repeated);
        EXPECT_GT(dot(scatter->direction, normal), 0.0f);
        EXPECT_NEAR(length(scatter->direction), 1.0f, 1e-6f);
        EXPECT_FLOAT_EQ(scatter->direction.x, repeated->direction.x);
        EXPECT_FLOAT_EQ(scatter->direction.y, repeated->direction.y);
        EXPECT_FLOAT_EQ(scatter->direction.z, repeated->direction.z);
        EXPECT_FLOAT_EQ(scatter->attenuation.x, 0.2f);
        EXPECT_FLOAT_EQ(scatter->attenuation.y, 0.4f);
        EXPECT_FLOAT_EQ(scatter->attenuation.z, 0.6f);
    }
    EXPECT_EQ(generator, replay);
    EXPECT_NE(generator, (mt19937{42}));
}

TEST(Material, SmoothMetalReflectsWithoutConsumingRandomSamples) {
    const Material material = Metal{Color{0.9f, 0.8f, 0.7f}};
    const Ray incoming{Vec3{}, Vec3{2, -2, 0}};
    const ShadingContext hit{1.0f, incoming.at(1.0f), Vec3{0, 1, 0}, material, incoming, true};
    mt19937 generator{42};
    const auto scatter = std::get<Metal>(material).scatter(hit, generator);

    ASSERT_TRUE(scatter);
    EXPECT_NEAR(scatter->direction.x, sqrt(0.5f), 1e-6f);
    EXPECT_NEAR(scatter->direction.y, sqrt(0.5f), 1e-6f);
    EXPECT_FLOAT_EQ(scatter->direction.z, 0.0f);
    EXPECT_FLOAT_EQ(scatter->attenuation.x, 0.9f);
    EXPECT_FLOAT_EQ(scatter->attenuation.y, 0.8f);
    EXPECT_FLOAT_EQ(scatter->attenuation.z, 0.7f);
    EXPECT_EQ(generator, (mt19937{42}));
}

TEST(Material, MetalAbsorbsReflectionsBelowSurface) {
    const Material material = Metal{};
    const Ray incoming{Vec3{}, Vec3{0, 1, 0}};
    const ShadingContext hit{1.0f, incoming.at(1.0f), Vec3{0, 1, 0}, material, incoming, true};
    mt19937 generator{42};
    EXPECT_FALSE(scatter(hit, generator));
}

TEST(Material, RoughMetalPerturbsReflection) {
    const Material material = Metal{Color{1, 1, 1}, 0.2f};
    const Ray incoming{Vec3{}, Vec3{0, 0, -1}};
    const ShadingContext hit{1.0f, incoming.at(1.0f), Vec3{0, 0, 1}, material, incoming, true};
    mt19937 generator{42};
    const auto scatter = std::get<Metal>(material).scatter(hit, generator);

    ASSERT_TRUE(scatter);
    EXPECT_GT(scatter->direction.z, 0.0f);
    EXPECT_GT(scatter->direction.x * scatter->direction.x + scatter->direction.y * scatter->direction.y, 0.0f);
    EXPECT_NEAR(length(scatter->direction), 1.0f, 1e-6f);
}

TEST(Material, DielectricUsesEnteringStateForRefraction) {
    const Material material = Dielectric{Color{1, 1, 1}, 1.5f};
    for (bool entering : {true, false}) {
        mt19937 generator{42};
        const Vec3 normal = entering ? Vec3{0, 0, 1} : Vec3{0, 0, -1};
        const float z = sqrt(1.0f - 0.3f * 0.3f);
        const Ray incoming{Vec3{}, Vec3{0.3f, 0, entering ? -z : z}};
        const ShadingContext hit{1.0f, incoming.at(1.0f), normal, material, incoming, entering};
        const float refracted_x = entering ? 0.3f / 1.5f : 0.3f * 1.5f;
        int refractions = 0;

        for (int i = 0; i < 32; ++i) {
            const auto scatter = std::get<Dielectric>(material).scatter(hit, generator);
            ASSERT_TRUE(scatter);
            EXPECT_NEAR(length(scatter->direction), 1.0f, 1e-6f);
            EXPECT_FLOAT_EQ(scatter->attenuation.x, 1.0f);
            EXPECT_FLOAT_EQ(scatter->attenuation.y, 1.0f);
            EXPECT_FLOAT_EQ(scatter->attenuation.z, 1.0f);
            if (dot(scatter->direction, normal) < 0.0f) {
                ++refractions;
                EXPECT_NEAR(scatter->direction.x, refracted_x, 1e-6f);
            } else {
                EXPECT_NEAR(scatter->direction.x, incoming.direction.x, 1e-6f);
                EXPECT_NEAR(scatter->direction.z, -incoming.direction.z, 1e-6f);
            }
        }
        EXPECT_GT(refractions, 0);
    }
}

TEST(Material, DielectricReflectsWhenExitingAboveCriticalAngle) {
    const Material material = Dielectric{};
    mt19937 generator{42};
    const Ray incoming{Vec3{}, Vec3{sqrt(0.75f), 0, 0.5f}};
    const ShadingContext hit{1.0f, incoming.at(1.0f), Vec3{0, 0, -1}, material, incoming, false};
    const auto scatter = std::get<Dielectric>(material).scatter(hit, generator);

    ASSERT_TRUE(scatter);
    EXPECT_NEAR(scatter->direction.x, incoming.direction.x, 1e-6f);
    EXPECT_FLOAT_EQ(scatter->direction.y, 0.0f);
    EXPECT_NEAR(scatter->direction.z, -0.5f, 1e-6f);
}
