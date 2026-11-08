#pragma once

#include "pathtracer/camera/image.hpp"
#include "pathtracer/geometry/vec3.hpp"

namespace pathtracer
{
    struct Viewport
    {
        // Coordinates are relative to the camera origin.
        Vec3 pixel00;
        Vec3 pixel_du;
        Vec3 pixel_dv;
        
        Viewport(const Image& image, float fov);
    };
} // namespace pathtracer
