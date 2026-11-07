#include "pathtracer/camera/image.hpp"

#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <type_traits>

using std::is_copy_assignable_v;
using std::is_move_assignable_v;
using std::logic_error;
using std::ostringstream;
using std::out_of_range;

using namespace pathtracer;

// Replacing a referenced Image would invalidate the Camera's cached geometry.
static_assert(!is_copy_assignable_v<Image>);
static_assert(!is_move_assignable_v<Image>);

TEST(Image, RejectsInvalidCoordinatesAndEmptyOutput)
{
    Image image{2, 1};
    EXPECT_THROW(image.set_pixel(0, 2, Vec3{}), out_of_range);
    EXPECT_THROW(image.set_pixel(1, 0, Vec3{}), out_of_range);

    ostringstream output;
    EXPECT_THROW(Image{}.write_ppm(output), logic_error);
}
