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

        const float viewport_height = 2.0f;
        const float focal_length = 1.0f;
        const float aspect_ratio = static_cast<float>(width_) / static_cast<float>(height_);
        const Vec3 viewport_u{viewport_height * aspect_ratio, 0, 0};
        const Vec3 viewport_v{0, -viewport_height, 0};
        const Vec3 viewport_center = origin_ + Vec3{0, 0, -focal_length};

        pixel_du_ = viewport_u / static_cast<float>(width_);
        pixel_dv_ = viewport_v / static_cast<float>(height_);
        pixel00_ = viewport_center - viewport_u / 2 - viewport_v / 2;
    }

    Ray Camera::generate_ray(std::size_t row, std::size_t col) const
    {
        const float pixel_center_offset = 0.5f;
        const Vec3 pixel_center = pixel00_ + pixel_dv_ * (row + pixel_center_offset)
                                 + pixel_du_ * (col + pixel_center_offset);

        return Ray{origin_, pixel_center - origin_};
    }

} // namespace pathtracer
