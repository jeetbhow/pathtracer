#include "pathtracer/renderer.hpp"

#include <gtest/gtest.h>
#include <sstream>
#include <string>

using namespace pathtracer;

class EnclosedRenderer : public testing::TestWithParam<MaterialType>
{
};

TEST_P(EnclosedRenderer, NoSkyLightLeaksThroughAClosedSphere)
{
    Sphere enclosure{Vec3{0, 0, 0}, 2};
    enclosure.material = Material{GetParam(), Vec3{1, 1, 1}, 0};
    const Renderer renderer{Scene{{enclosure}}, Camera{3, 3, 32}, 42};
    std::ostringstream output;
    renderer.render().write_ppm(output);

    std::istringstream pixels{output.str()};
    std::string magic;
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

INSTANTIATE_TEST_SUITE_P(Materials, EnclosedRenderer,
                        testing::Values(MaterialType::Diffuse, MaterialType::Metal));
