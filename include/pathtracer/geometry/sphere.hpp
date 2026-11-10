#pragma once

#include "pathtracer/geometry/intersection.hpp"
#include "pathtracer/geometry/ray.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <optional>
#include <random>

namespace pathtracer
{
    struct Sphere
    {
        Vec3 position;
        float radius;

        std::optional<Intersection> intersect(const Ray& ray, float t_min = 0.001f) const;
        Vec3 normal_at(Vec3 point) const;
        Vec3 random_point(std::mt19937& generator) const;
    };
} // namespace pathtracer
