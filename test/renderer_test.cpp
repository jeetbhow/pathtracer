#include "pathtracer/renderer.hpp"

#include <gtest/gtest.h>
#include <sstream>

using namespace pathtracer;

TEST(Renderer, SelectsNearestSphereRegardlessOfOrderAndIgnoresMisses)
{
    const Camera camera{1, 1, 16, 42};
    // Both spheres cover the pixel's field of view, with the near surface in front.
    const Sphere near{Vec3{0.5f, 0, -3}, 2.9f};
    const Sphere far{Vec3{0, 0, -8}, 7.0f};
    const Sphere miss{Vec3{10, 0, -3}, 1.0f};

    for (const Ray& ray : camera.generate_rays(0, 0))
    {
        const auto near_hit = near.hit(ray);
        const auto far_hit = far.hit(ray);
        ASSERT_TRUE(near_hit.has_value());
        ASSERT_TRUE(far_hit.has_value());
        ASSERT_LT(near_hit->t, far_hit->t);
        ASSERT_FALSE(miss.hit(ray).has_value());
    }

    std::ostringstream near_output;
    render(camera, Scene{{near}}).write_ppm(near_output);
    std::ostringstream far_output;
    render(camera, Scene{{far}}).write_ppm(far_output);
    ASSERT_NE(near_output.str(), far_output.str());

    std::ostringstream near_first;
    render(camera, Scene{{near, far, miss}}).write_ppm(near_first);
    std::ostringstream far_first;
    render(camera, Scene{{far, miss, near}}).write_ppm(far_first);

    EXPECT_EQ(near_first.str(), near_output.str());
    EXPECT_EQ(far_first.str(), near_output.str());
}

TEST(Renderer, EmptySceneAndMissedObjectsProduceTheSameBackground)
{
    const Camera camera{3, 3, 4, 42};
    const Scene scene{{Sphere{Vec3{0, 0, 3}, 1.0f}}};

    std::ostringstream empty_output;
    render(camera, Scene{}).write_ppm(empty_output);
    std::ostringstream missed_output;
    render(camera, scene).write_ppm(missed_output);

    EXPECT_EQ(missed_output.str(), empty_output.str());
}
