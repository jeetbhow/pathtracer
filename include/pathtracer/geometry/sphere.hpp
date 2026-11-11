#pragma once

#include "pathtracer/geometry/geometry.hpp"

#include <optional>
#include <random>

namespace pathtracer
{
    struct Sphere : public Geometry
    {
        float radius;

        Sphere() = default;
        Sphere(Vec3 position, float radius);

        std::optional<Intersection> intersect(const Ray& ray, float t_min = 0.001f) const override;
        Vec3 normal_at(Vec3 point) const override;
        Vec3 random_point(std::mt19937& generator) const;
    };
} // namespace pathtracer
