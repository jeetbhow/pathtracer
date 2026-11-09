#include "pathtracer/camera/viewport.hpp"

#include <cmath>
#include <cstddef>
#include <numbers>
#include <stdexcept>

using std::invalid_argument;
using std::numbers::pi_v;
using std::size_t;
using std::tan;

namespace pathtracer
{
    Viewport::Viewport(const Image& image, Vec3 up, Vec3 direction, float fov, float focal_length)
    {
        const size_t img_width = image.width;
        const size_t img_height = image.height;

        if (img_width == 0 || img_height == 0)
        {
            throw invalid_argument("Viewport image dimensions must be nonzero");
        }

        const float aspect_ratio = static_cast<float>(img_width) / static_cast<float>(img_height);
        const float fov_radians = fov * pi_v<float> / 180.0f;
        const float vp_height = 2.0f * focal_length * tan(fov_radians / 2.0f);
        const float vp_width = vp_height * aspect_ratio;

        const Vec3 forward = normalize(direction);
        const Vec3 right = normalize(cross(forward, up));
        const Vec3 down = normalize(cross(forward, right));
        const Vec3 center = focal_length * forward;

        const Vec3 u = vp_width * right;
        const Vec3 v = vp_height * down;

        pixel_du = u / static_cast<float>(img_width);
        pixel_dv = v / static_cast<float>(img_height);
        pixel00 = center - u / 2 - v / 2;
    }
} // namespace pathtracer
