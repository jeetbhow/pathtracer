#include "pathtracer/geometry/sphere.hpp"
#include "pathtracer/renderer.hpp"
#include "pathtracer/object/material.hpp"

#include <memory>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

using std::istringstream;
using std::ostringstream;
using std::string;

using namespace pathtracer;

template <typename T>
class EnclosedRenderer : public testing::Test
{
};

using EnclosedMaterials = testing::Types<Diffuse, Metal>;
TYPED_TEST_SUITE(EnclosedRenderer, EnclosedMaterials);

TYPED_TEST(EnclosedRenderer, NoSkyLightLeaksThroughAClosedSphere)
{
    Scene scene;
    scene.objects.emplace_back(std::make_unique<Sphere>(Vec3{0, 0, 0}, 2), TypeParam{});
    Image image{3, 3};
    Camera camera{image};
    camera.samples = 32;
    camera.initialize();
    const Renderer renderer{scene, camera, 42};
    EXPECT_EQ(&renderer.render(), &image);
    ostringstream output;
    renderer.render().write_ppm(output);

    istringstream pixels{output.str()};
    string magic;
    int width, height, maximum;
    ASSERT_TRUE(pixels >> magic >> width >> height >> maximum);
    ASSERT_EQ(magic, "P3");
    ASSERT_EQ(width, 3);
    ASSERT_EQ(height, 3);
    ASSERT_EQ(maximum, 255);
    for (int i = 0; i < width * height; ++i)
    {
        int red, green, blue;
        ASSERT_TRUE(pixels >> red >> green >> blue);
        EXPECT_EQ(red, 0);
        EXPECT_EQ(green, 0);
        EXPECT_EQ(blue, 0);
    }
}

TEST(Renderer, UsesTheReferencedScene)
{
    Scene scene;
    Image image{1, 1};
    Camera camera{image};
    camera.samples = 1;
    camera.initialize();
    const Renderer renderer{scene, camera, 42};

    scene.objects.emplace_back(std::make_unique<Sphere>(Vec3{0, 0, 0}, 2), Diffuse{Color{0, 0, 0}});

    ostringstream output;
    renderer.render().write_ppm(output);
    EXPECT_EQ(output.str(), "P3\n1 1\n255\n0 0 0\n");
}
