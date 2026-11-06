#include "pathtracer/camera.hpp"

#include <stdexcept>
#include <random>

using std::vector;
using std::random_device;
using std::mt19937;
using std::uniform_real_distribution;

namespace pathtracer
{
    Camera::Camera(std::size_t width, std::size_t height, int samples_per_pixel)
        : Camera(width, height, samples_per_pixel, random_device{}())
    {
    }

    Camera::Camera(std::size_t width, std::size_t height, int samples_per_pixel, std::uint32_t seed)
        : width_(width), height_(height), samples_per_pixel_(samples_per_pixel), seed_(seed)
    {
        if (width_ == 0 || height_ == 0)
        {
            throw std::invalid_argument("Camera image dimensions must be nonzero");
        }
        if (samples_per_pixel_ <= 0)
        {
            throw std::invalid_argument("Sample count must be positive");
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

    vector<Ray> Camera::generate_rays(std::size_t row, std::size_t col) const
    {
        vector<Ray> rays;
        rays.reserve(static_cast<std::size_t>(samples_per_pixel_));

        std::seed_seq seed{seed_, static_cast<std::uint32_t>(row), static_cast<std::uint32_t>(col)};
        mt19937 gen{seed};
        uniform_real_distribution<float> dist(0, 1);

        for (int i = 0; i < samples_per_pixel_; i++)
        {
            const float col_offset = dist(gen);
            const float row_offset = dist(gen);
            const Vec3 pixel_center = pixel00_ +
                                      pixel_dv_ * (row + row_offset) +
                                      pixel_du_ * (col + col_offset);
            rays.emplace_back(origin_, pixel_center - origin_);
        }

        return rays;
    }

} // namespace pathtracer
