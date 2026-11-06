#pragma once

#include "pathtracer/geometry/ray.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace pathtracer
{

    class Camera
    {
    public:
        Camera(std::size_t width, std::size_t height, int samples_per_pixel = 8);
        Camera(std::size_t width, std::size_t height, int samples_per_pixel, std::uint32_t seed);

        std::size_t width() const
        {
            return width_;
        }

        std::size_t height() const
        {
            return height_;
        }

        int samples_per_pixel() const
        {
            return samples_per_pixel_;
        }

        Ray generate_ray(std::size_t row, std::size_t col) const;
        std::vector<Ray> generate_rays(std::size_t row, std::size_t col) const;

    private:
        Vec3 origin_ = Vec3{0, 0, 0};
        std::size_t width_;
        std::size_t height_;
        int samples_per_pixel_;
        std::uint32_t seed_;
        Vec3 pixel00_;
        Vec3 pixel_du_;
        Vec3 pixel_dv_;
    };

} // namespace pathtracer
