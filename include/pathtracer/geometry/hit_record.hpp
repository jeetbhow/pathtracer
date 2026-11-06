#pragma once

#include "pathtracer/geometry/vec3.hpp"

namespace pathtracer
{
    class Sphere;

    struct HitRecord
    {
        float t;
        Vec3 position;
        Vec3 normal;
        const Sphere& object;
    };
} // namespace pathtracer
