#include "pathtracer/camera.hpp"

#include <stdexcept>

namespace pathtracer
{

    Camera::Camera(std::size_t width, std::size_t height) : width_(width), height_(height)
    {
        if (width_ == 0 || height_ == 0)
        {
            throw std::invalid_argument("Camera image dimensions must be nonzero");
        }
    }

    Ray Camera::generate_ray(std::size_t row, std::size_t col) const
    {
        const float jitter = 0.5f;
        const float aspect_ratio = static_cast<float>(width_) / static_cast<float>(height_);

        Vec3 vp_u = Vec3{vp_height_ * aspect_ratio, 0, 0};
        Vec3 vp_v = Vec3{0, -vp_height_, 0};
        Vec3 pixel_du = vp_u / static_cast<float>(width_);
        Vec3 pixel_dv = vp_v / static_cast<float>(height_);
        Vec3 center = origin_ + Vec3{0, 0, -focal_len_};
        Vec3 pixel00 = center - vp_u / 2 - vp_v / 2;

        Vec3 pixel_center = pixel00 + pixel_dv * (row + jitter) + pixel_du * (col + jitter);

        return Ray { origin_, pixel_center };
    }

} // namespace pathtracer
