#include "pathtracer/camera/camera.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <cmath>
#include <cstddef>
#include <numbers>
#include <stdexcept>

using std::invalid_argument;
using std::mt19937;
using std::numbers::pi_v;
using std::size_t;
using std::tan;
using std::uniform_real_distribution;

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

        const Vec3 forward = direction;
        const Vec3 right = normalize(cross(forward, up));
        const Vec3 down = normalize(cross(forward, right));
        const Vec3 center = focal_length * forward;

        const Vec3 u = vp_width * right;
        const Vec3 v = vp_height * down;

        pixel_du = u / static_cast<float>(img_width);
        pixel_dv = v / static_cast<float>(img_height);
        pixel00 = center - u / 2 - v / 2;
    }

    Camera::Camera(Image& image) : image_(image)
    {
    }

    void Camera::initialize()
    {
        const Vec3 direction = normalize(look_at - position);
        if (length_squared(direction) == 0.0f)
        {
            throw std::invalid_argument("Camera look_at must differ from position");
        }

        viewport_ = Viewport{image_, up, direction, fov, focal_length};
        focus_plane_ = FocusPlane{direction};
    }

    Ray Camera::generate_ray(size_t row, size_t col, mt19937& generator) const
    {
        uniform_real_distribution<float> distribution{0.0f, 1.0f};
        const float col_offset = distribution(generator);
        const float row_offset = distribution(generator);
        const Vec3 pixel_sample = viewport_.pixel00 + 
                                  viewport_.pixel_dv * (row + row_offset) + 
                                  viewport_.pixel_du * (col + col_offset);
        return Ray{position, pixel_sample};
    }
} // namespace pathtracer
