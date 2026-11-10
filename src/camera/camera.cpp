#include "pathtracer/camera/camera.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>

using std::invalid_argument;
using std::mt19937;
using std::size_t;
using std::tan;
using std::uniform_real_distribution;

namespace pathtracer
{
    Camera::Camera(Image& image) : image_(image)
    {
    }

    void Camera::initialize()
    {
        const Vec3 forward = normalize(look_at - position);
        if (length_squared(forward) == 0.0f)
        {
            throw invalid_argument("Camera look_at must differ from position");
        }

        initialize_viewport(forward);
        aperture_ = Circle{position, forward, DEFAULT_APERTURE_RADIUS};
        focus_plane_ = Plane{position + focus_distance * forward, forward};
    }

    void Camera::initialize_viewport(Vec3 forward)
    {
        const size_t img_width = image_.width;
        const size_t img_height = image_.height;
        if (img_width == 0 || img_height == 0)
        {
            throw invalid_argument("Camera image dimensions must be nonzero");
        }

        const float aspect_ratio = static_cast<float>(img_width) / static_cast<float>(img_height);
        const float fov_radians = to_radians(fov);
        const float vp_height = 2.0f * focal_length * tan(fov_radians / 2.0f);
        const float vp_width = vp_height * aspect_ratio;

        const Vec3 up = std::abs(dot(forward, DEFAULT_UP)) < 0.99f ? DEFAULT_UP : Vec3{0, 0, 1};
        const Vec3 right = normalize(cross(forward, up));

        if (length_squared(right) == 0.0f)
        {
            throw invalid_argument("Camera up vector must not be parallel to the viewing direction");
        }

        const Vec3 down = normalize(cross(forward, right));
        const Vec3 center = focal_length * forward;

        const Vec3 u = vp_width * right;
        const Vec3 v = vp_height * down;

        pixel_du_ = u / static_cast<float>(img_width);
        pixel_dv_ = v / static_cast<float>(img_height);
        pixel00_ = center - u / 2 - v / 2;
    }

    Ray Camera::generate_ray(size_t row, size_t col, mt19937& generator) const
    {
        uniform_real_distribution<float> distribution{0.0f, 1.0f};
        const float col_offset = distribution(generator);
        const float row_offset = distribution(generator);
        const Vec3 pixel_sample = pixel00_ +
                                  pixel_dv_ * (row + row_offset) +
                                  pixel_du_ * (col + col_offset);
        return Ray{position, pixel_sample};
    }
} // namespace pathtracer
