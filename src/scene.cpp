#include "scene.hpp"

#include "image.hpp"

#include <stdexcept>

Camera::Camera(Image& image) : image_(image)
{
    if (image_.width() == 0 || image_.height() == 0)
    {
        throw std::invalid_argument("Camera image dimensions must be nonzero");
    }
}

Color ray_color(const Ray& ray)
{
    const Vec3 unit_direction = ray.direction().normalized();
    const float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * Color{1.0f, 1.0f, 1.0f} + t * Color{0.5f, 0.7f, 1.0f};
}

Ray Camera::generate_ray(std::size_t row, std::size_t col) const
{
    const float jitter = 0.5f;
    const float aspect_ratio = static_cast<float>(image_.width()) / static_cast<float>(image_.height());
    Vec3 vp_u = Vec3{vp_height_ * aspect_ratio, 0, 0};
    Vec3 vp_v = Vec3{0, -vp_height_, 0};
    Vec3 pixel_du = vp_u / static_cast<float>(image_.width());
    Vec3 pixel_dv = vp_v / static_cast<float>(image_.height());
    Vec3 center = origin_ + Vec3{0, 0, -focal_len_};
    Vec3 pixel00 = center - vp_u / 2 - vp_v / 2;

    Vec3 pixel_center = pixel00 + pixel_dv * (row + jitter) + pixel_du * (col + jitter);

    return Ray { origin_, pixel_center };
}
