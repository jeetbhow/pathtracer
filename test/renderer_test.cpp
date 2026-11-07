#include "pathtracer/renderer.hpp"
#include "pathtracer/object/material.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <string>

using std::istringstream;
using std::make_shared;
using std::mt19937;
using std::nullopt;
using std::optional;
using std::ostringstream;
using std::shared_ptr;
using std::string;

using namespace pathtracer;

class AbsorbingMaterial : public Material
{
public:
    optional<ScatterResult> scatter(const Ray&, const Vec3&, bool, mt19937&) const override
    {
        return nullopt;
    }
};

class EnclosedRenderer : public testing::TestWithParam<shared_ptr<const Material>>
{
};

TEST_P(EnclosedRenderer, NoSkyLightLeaksThroughAClosedSphere)
{
    Sphere enclosure{Vec3{0, 0, 0}, 2};
    enclosure.material = GetParam();
    Image image{3, 3};
    const Renderer renderer{Scene{{make_shared<Sphere>(enclosure)}}, Camera{image, 32}, 42};
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

INSTANTIATE_TEST_SUITE_P(Materials, EnclosedRenderer,
                        testing::Values(make_shared<Diffuse>(Color{1, 1, 1}),
                                        make_shared<Metal>(Color{1, 1, 1}, 0),
                                        make_shared<AbsorbingMaterial>()));
