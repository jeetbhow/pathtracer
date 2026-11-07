#include "pathtracer/camera/viewport.hpp"

#include <stdexcept>

using std::invalid_argument;
using std::size_t;

namespace pathtracer
{
    Viewport::Viewport(size_t image_width, size_t image_height)
    {
        if (image_width == 0 || image_height == 0)
        {
            throw invalid_argument("Viewport image dimensions must be nonzero");
        }

        const float height = 2.0f;
        const float focal_length = 1.0f;
        const float aspect_ratio = static_cast<float>(image_width) / static_cast<float>(image_height);
        const Vec3 u{height * aspect_ratio, 0, 0};
        const Vec3 v{0, -height, 0};
        const Vec3 center{0, 0, -focal_length};

        pixel_du = u / static_cast<float>(image_width);
        pixel_dv = v / static_cast<float>(image_height);
        pixel00 = center - u / 2 - v / 2;
    }
} // namespace pathtracer
