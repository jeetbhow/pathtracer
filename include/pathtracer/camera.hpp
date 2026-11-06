#pragma once

#include "pathtracer/geometry/ray.hpp"

#include <cstddef>

namespace pathtracer
{

    class Camera
    {
    public:
        Camera(std::size_t width, std::size_t height);

        std::size_t width() const
        {
            return width_;
        }
        std::size_t height() const
        {
            return height_;
        }
        Ray generate_ray(std::size_t row, std::size_t col) const;

    private:
        Vec3 origin_ = Vec3{0, 0, 0};
        std::size_t width_;
        std::size_t height_;
        Vec3 pixel00_;
        Vec3 pixel_du_;
        Vec3 pixel_dv_;
    };

} // namespace pathtracer
