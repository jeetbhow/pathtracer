#include "image.hpp"

#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>

TEST(Image, WritesPixelsInRowMajorPpmFormat)
{
    Image image{2, 2};
    image.set_pixel(0, 0, Vec3{1.0f, 0.0f, 0.5f});
    image.set_pixel(0, 1, Vec3{-1.0f, 2.0f, 0.0f});
    image.set_pixel(1, 0, Vec3{0.0f, 0.0f, 1.0f});

    std::ostringstream output;
    image.write_ppm(output);
    EXPECT_EQ(output.str(), "P3\n2 2\n255\n255 0 128\n0 255 0\n0 0 255\n0 0 0\n");
}

TEST(Image, RejectsInvalidCoordinatesAndEmptyOutput)
{
    Image image{2, 1};
    EXPECT_THROW(image.set_pixel(0, 2, Vec3{}), std::out_of_range);
    EXPECT_THROW(image.set_pixel(1, 0, Vec3{}), std::out_of_range);

    std::ostringstream output;
    EXPECT_THROW(Image{}.write_ppm(output), std::logic_error);
}
