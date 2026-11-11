#include "pathtracer/geometry/plane.hpp"
#include "pathtracer/object/object.hpp"

#include <gtest/gtest.h>
#include <array>
#include <cmath>
#include <memory>
#include <random>
#include <utility>

using std::mt19937;

using namespace pathtracer;

TEST(Circle, PreservesRadiusCheckThroughPlaneInterface) {
    std::unique_ptr<Plane> geometry = std::make_unique<Circle>(Vec3{0, 0, -3}, Vec3{0, 0, 1}, 1);
    const Object circle{std::move(geometry), Diffuse{}};

    const Ray inside{Vec3{0.5f, 0, 0}, Vec3{0, 0, -1}};
    const auto hit = circle.hit(inside);
    ASSERT_TRUE(hit);
    EXPECT_FLOAT_EQ(hit->t, 3.0f);
    EXPECT_EQ(hit->normal, (Vec3{0, 0, 1}));
    EXPECT_EQ(&hit->material, &circle.material);
    EXPECT_FALSE(circle.hit(inside, 3.0f));
    EXPECT_FALSE(circle.hit(Ray{Vec3{2, 0, 0}, Vec3{0, 0, -1}}));
}

TEST(Circle, RandomPointUsesSuppliedGeneratorReproducibly) {
    const Circle circle{Vec3{2, -3, 5}, normalize(Vec3{1, 2, 3}), 2};
    mt19937 generator{42};
    mt19937 replay{42};
    const mt19937 initial = generator;

    const Vec3 first = circle.random_point(generator);
    const Vec3 repeated_first = circle.random_point(replay);
    EXPECT_FLOAT_EQ(first.x, repeated_first.x);
    EXPECT_FLOAT_EQ(first.y, repeated_first.y);
    EXPECT_FLOAT_EQ(first.z, repeated_first.z);

    bool different_point_seen = false;
    for (int sample = 0; sample < 128; ++sample) {
        const Vec3 point = circle.random_point(generator);
        const Vec3 repeated = circle.random_point(replay);
        EXPECT_FLOAT_EQ(point.x, repeated.x);
        EXPECT_FLOAT_EQ(point.y, repeated.y);
        EXPECT_FLOAT_EQ(point.z, repeated.z);
        different_point_seen |= length_squared(point - first) > 0.0f;
    }

    EXPECT_TRUE(different_point_seen);
    EXPECT_EQ(generator, replay);
    EXPECT_NE(generator, initial);
}

TEST(Circle, RandomPointsLieOnPlaneAndWithinRadius) {
    const std::array normals{
        Vec3{1, 0, 0},
        Vec3{-1, 0, 0},
        Vec3{0, 1, 0},
        Vec3{0, -1, 0},
        Vec3{0, 0, 1},
        Vec3{0, 0, -1},
        normalize(Vec3{1, 2, 3}),
        normalize(Vec3{-2, 3, -1}),
    };

    for (const Vec3 normal : normals) {
        for (const float radius : {0.5f, 3.0f}) {
            SCOPED_TRACE(testing::Message() << "normal = (" << normal.x << ", "
                                            << normal.y << ", " << normal.z << "), radius = " << radius);
            const Circle circle{Vec3{3.25f, -4, 8}, normal, radius};
            mt19937 generator{1234};

            for (int sample = 0; sample < 512; ++sample) {
                SCOPED_TRACE(sample);
                const Vec3 point = circle.random_point(generator);
                ASSERT_TRUE(std::isfinite(point.x));
                ASSERT_TRUE(std::isfinite(point.y));
                ASSERT_TRUE(std::isfinite(point.z));
                const Vec3 offset = point - circle.position;
                EXPECT_NEAR(dot(offset, normal), 0.0f, 1e-5f);
                EXPECT_LE(length_squared(offset), radius * radius + 1e-5f);
            }
        }
    }
}

TEST(Circle, RandomPointsDoNotCollapseAtOriginOrAlongNormal) {
    for (const Vec3 position : {Vec3{0, 0, 0}, Vec3{0, 0, 5}}) {
        const Circle circle{position, Vec3{0, 0, 1}, 2};
        mt19937 generator{42};
        bool noncentral_point_seen = false;

        for (int sample = 0; sample < 128; ++sample) {
            const Vec3 offset = circle.random_point(generator) - position;
            EXPECT_NEAR(offset.z, 0.0f, 1e-5f);
            EXPECT_LE(length_squared(offset), 4.0f + 1e-5f);
            noncentral_point_seen |= length_squared(offset) > 0.0f;
        }

        EXPECT_TRUE(noncentral_point_seen);
    }
}

TEST(Circle, RandomPointsAreUniformOverDiskArea) {
    const Circle circle{Vec3{3, -2, 7}, Vec3{0, 0, 1}, 3};
    mt19937 generator{2026};
    constexpr int sample_count = 20000;
    double sum_x = 0;
    double sum_y = 0;
    double sum_x_squared = 0;
    double sum_y_squared = 0;
    int inner_half_radius_count = 0;
    std::array<int, 4> quadrant_counts{};

    for (int sample = 0; sample < sample_count; ++sample) {
        const Vec3 offset = (circle.random_point(generator) - circle.position) / circle.radius;
        ASSERT_TRUE(std::isfinite(offset.x));
        ASSERT_TRUE(std::isfinite(offset.y));
        const double x = offset.x;
        const double y = offset.y;
        sum_x += x;
        sum_y += y;
        sum_x_squared += x * x;
        sum_y_squared += y * y;
        if (x * x + y * y <= 0.25) {
            ++inner_half_radius_count;
        }
        ++quadrant_counts[(x >= 0 ? 1 : 0) + (y >= 0 ? 2 : 0)];
    }

    // Uniform disk area gives E[x] = E[y] = 0 and E[x^2] = E[y^2] = R^2 / 4.
    // These coordinates have been divided by R. Broad bounds allow sample noise.
    EXPECT_NEAR(sum_x / sample_count, 0.0, 0.02);
    EXPECT_NEAR(sum_y / sample_count, 0.0, 0.02);
    EXPECT_NEAR(sum_x_squared / sample_count, 0.25, 0.02);
    EXPECT_NEAR(sum_y_squared / sample_count, 0.25, 0.02);

    // Half the radius encloses one quarter of the area, as does each quadrant.
    EXPECT_NEAR(static_cast<double>(inner_half_radius_count) / sample_count, 0.25, 0.02);
    for (const int count : quadrant_counts) {
        EXPECT_NEAR(static_cast<double>(count) / sample_count, 0.25, 0.02);
    }
}
