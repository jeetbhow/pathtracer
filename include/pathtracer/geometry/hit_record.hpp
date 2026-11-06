#pragma once

#include "pathtracer/geometry/ray.hpp"

namespace pathtracer
{
    class Sphere;

    struct HitRecord
    {
        float t;
        Vec3 position;
        Vec3 normal;
        const Sphere& object;
        Ray incoming;
    };
} // namespace pathtracer
