#pragma once

#include "pathtracer/geometry/ray.hpp"
#include "pathtracer/camera/image.hpp"
#include "pathtracer/camera/viewport.hpp"

#include <cstddef>
#include <random>

namespace pathtracer
{
    class Camera
    {
    public:
        // Non-owning: image must outlive this Camera and any Renderer holding a copy.
        // The reference stays bound to one Image, whose dimensions are immutable.
        Image& image;
        const unsigned int samples;
        Vec3 origin = Vec3{0, 0, 0};

        Camera(Image& image, unsigned int samples = 8);

        Ray generate_ray(std::size_t row, std::size_t col, std::mt19937& generator) const;

    private:
        Viewport viewport_;
    };

} // namespace pathtracer
