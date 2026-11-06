#include "pathtracer/renderer.hpp"

#include <gtest/gtest.h>
#include <sstream>

using namespace pathtracer;

TEST(Renderer, SelectsNearestSphereRegardlessOfOrderAndIgnoresMisses)
{
    const Camera camera{1, 1, 16};
    // Both spheres cover the pixel's field of view, with the near surface in front.
    const Sphere near{Vec3{0.5f, 0, -3}, 2.9f};
    const Sphere far{Vec3{0, 0, -8}, 7.0f};
    const Sphere miss{Vec3{10, 0, -3}, 1.0f};

    std::mt19937 generator{42};
    for (const Ray& ray : camera.generate_rays(0, 0, generator))
    {
        const auto near_hit = near.hit(ray);
        const auto far_hit = far.hit(ray);
        ASSERT_TRUE(near_hit.has_value());
        ASSERT_TRUE(far_hit.has_value());
        ASSERT_LT(near_hit->t, far_hit->t);
        ASSERT_FALSE(miss.hit(ray).has_value());
    }

    std::ostringstream near_output;
    Renderer{Scene{{near}}, camera, 42}.render().write_ppm(near_output);
    std::ostringstream far_output;
    Renderer{Scene{{far}}, camera, 42}.render().write_ppm(far_output);
    ASSERT_NE(near_output.str(), far_output.str());

    std::ostringstream near_first;
    Renderer{Scene{{near, far, miss}}, camera, 42}.render().write_ppm(near_first);
    std::ostringstream far_first;
    Renderer{Scene{{far, miss, near}}, camera, 42}.render().write_ppm(far_first);

    EXPECT_EQ(near_first.str(), near_output.str());
    EXPECT_EQ(far_first.str(), near_output.str());
}

TEST(Renderer, EmptySceneAndMissedObjectsProduceTheSameBackground)
{
    const Camera camera{3, 3, 4};
    const Scene scene{{Sphere{Vec3{0, 0, 3}, 1.0f}}};

    std::ostringstream empty_output;
    Renderer{Scene{}, camera, 42}.render().write_ppm(empty_output);
    std::ostringstream missed_output;
    Renderer{scene, camera, 42}.render().write_ppm(missed_output);

    EXPECT_EQ(missed_output.str(), empty_output.str());
}
