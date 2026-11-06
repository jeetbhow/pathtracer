#pragma once

#include "pathtracer/geometry/vec3.hpp"

namespace pathtracer
{
    struct HitRecord
    {
        float t;
        Vec3 position;
        Vec3 normal;
    };
} // namespace pathtracer
