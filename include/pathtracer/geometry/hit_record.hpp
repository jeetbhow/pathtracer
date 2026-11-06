#pragma once

#include "pathtracer/geometry/ray.hpp"

namespace pathtracer
{
    class Sphere;

    struct HitRecord
    {
        float t;
        Vec3 position;
        Vec3 normal; // Shading normal, oriented against the incoming ray.
        const Sphere& object;
        Ray incoming;
    };
} // namespace pathtracer
