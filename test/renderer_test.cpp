#include "pathtracer/renderer.hpp"
#include "pathtracer/object/material.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <string>

using namespace pathtracer;

class AbsorbingMaterial : public Material
{
public:
    std::optional<ScatterResult> scatter(const Ray&, const Vec3&, bool, std::mt19937&) const override
    {
        return std::nullopt;
    }
};

class EnclosedRenderer : public testing::TestWithParam<std::shared_ptr<const Material>>
{
};

TEST_P(EnclosedRenderer, NoSkyLightLeaksThroughAClosedSphere)
{
    Sphere enclosure{Vec3{0, 0, 0}, 2};
    enclosure.material = GetParam();
    Image image{3, 3};
    const Renderer renderer{Scene{{std::make_shared<Sphere>(enclosure)}}, Camera{image, 32}, 42};
    EXPECT_EQ(&renderer.render(), &image);
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
                        testing::Values(std::make_shared<Diffuse>(Color{1, 1, 1}),
                                        std::make_shared<Metal>(Color{1, 1, 1}, 0),
                                        std::make_shared<AbsorbingMaterial>()));
