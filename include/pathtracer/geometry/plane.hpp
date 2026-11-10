#pragma once

#include "pathtracer/geometry/intersection.hpp"
#include "pathtracer/geometry/ray.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <optional>
#include <random>

namespace pathtracer
{
    struct Plane
    {
        Vec3 position{0, 0, 0};
        Vec3 normal{0, 1, 0};

        Plane() = default;
        Plane(Vec3 position, Vec3 normal);

        std::optional<Intersection> intersect(const Ray& ray, float t_min = 0.001f) const;
        Vec3 normal_at(Vec3 point) const;
    };

    struct Circle : public Plane
    {
        float radius = 1.0f;

        Circle() = default;
        Circle(Vec3 position, Vec3 normal, float radius);

        std::optional<Intersection> intersect(const Ray& ray, float t_min = 0.001f) const;
        Vec3 random_point(std::mt19937& generator) const;
    };
} // namespace pathtracer
