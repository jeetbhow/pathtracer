#pragma once

#include "pathtracer/geometry/vec3.hpp"

namespace pathtracer
{
    struct Intersection
    {
        float t;
        Vec3 position;
        Vec3 outward_normal;
    };
} // namespace pathtracer
