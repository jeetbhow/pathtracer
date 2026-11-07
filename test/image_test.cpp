#include "pathtracer/camera/image.hpp"

#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <type_traits>

using namespace pathtracer;

// Replacing a referenced Image would invalidate the Camera's cached geometry.
static_assert(!std::is_copy_assignable_v<Image>);
static_assert(!std::is_move_assignable_v<Image>);

TEST(Image, RejectsInvalidCoordinatesAndEmptyOutput)
{
    Image image{2, 1};
    EXPECT_THROW(image.set_pixel(0, 2, Vec3{}), std::out_of_range);
    EXPECT_THROW(image.set_pixel(1, 0, Vec3{}), std::out_of_range);

    std::ostringstream output;
    EXPECT_THROW(Image{}.write_ppm(output), std::logic_error);
}
