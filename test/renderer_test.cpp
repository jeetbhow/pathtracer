#include "pathtracer/renderer.hpp"

#include <gtest/gtest.h>
#include <sstream>

using namespace pathtracer;

TEST(Renderer, RendersSphereAndSkyToCameraDimensions)
{
    const Camera camera{3, 1};
    const Sphere sphere{Vec3{0, 0, -3}, 1.0f};

    const Image image = render(camera, sphere);
    EXPECT_EQ(image.width(), 3u);
    EXPECT_EQ(image.height(), 1u);

    std::ostringstream output;
    image.write_ppm(output);
    EXPECT_EQ(output.str(), "P3\n3 1\n255\n191 217 255\n255 0 0\n191 217 255\n");
}

TEST(Renderer, RayColorBlendsSkyFromWhiteToBlue)
{
    const Vec3 origin{0, 0, 0};
    const Color down = skybox_color(Ray{origin, Vec3{0, -2, 0}});
    const Color up = skybox_color(Ray{origin, Vec3{0, 2, 0}});

    EXPECT_FLOAT_EQ(down.x, 1.0f);
    EXPECT_FLOAT_EQ(down.y, 1.0f);
    EXPECT_FLOAT_EQ(down.z, 1.0f);
    EXPECT_FLOAT_EQ(up.x, 0.5f);
    EXPECT_FLOAT_EQ(up.y, 0.7f);
    EXPECT_FLOAT_EQ(up.z, 1.0f);
}
