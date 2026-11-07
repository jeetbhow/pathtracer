#pragma once

#include "pathtracer/geometry/vec3.hpp"

#include <cstddef>

namespace pathtracer
{
    struct Viewport
    {
        // Coordinates are relative to the camera origin.
        Vec3 pixel00;
        Vec3 pixel_du;
        Vec3 pixel_dv;
        
        Viewport(std::size_t image_width, std::size_t image_height);
    };
} // namespace pathtracer
