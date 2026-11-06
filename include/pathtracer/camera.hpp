#pragma once

#include "pathtracer/geometry/ray.hpp"

#include <cstddef>
#include <random>
#include <vector>

namespace pathtracer
{

    class Camera
    {
    public:
        Camera(std::size_t width, std::size_t height, int samples_per_pixel = 8);

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
        Ray generate_ray(std::size_t row, std::size_t col, std::mt19937& generator) const;
        std::vector<Ray> generate_rays(std::size_t row, std::size_t col, std::mt19937& generator) const;

    private:
        Vec3 origin_ = Vec3{0, 0, 0};
        std::size_t width_;
        std::size_t height_;
        int samples_per_pixel_;
        Vec3 pixel00_;
        Vec3 pixel_du_;
        Vec3 pixel_dv_;
    };

} // namespace pathtracer
