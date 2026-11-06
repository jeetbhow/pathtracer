#include "pathtracer/image.hpp"

#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>

using namespace pathtracer;

TEST(Image, RejectsInvalidCoordinatesAndEmptyOutput)
{
    Image image{2, 1};
    EXPECT_THROW(image.set_pixel(0, 2, Vec3{}), std::out_of_range);
    EXPECT_THROW(image.set_pixel(1, 0, Vec3{}), std::out_of_range);

    std::ostringstream output;
    EXPECT_THROW(Image{}.write_ppm(output), std::logic_error);
}
