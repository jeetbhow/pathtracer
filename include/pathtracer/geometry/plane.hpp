#pragma once

#include "pathtracer/geometry/geometry.hpp"

#include <optional>
#include <random>

namespace pathtracer {
    struct Plane : public Geometry {
        Vec3 normal{0, 1, 0};

        Plane() = default;
        Plane(Vec3 position, Vec3 normal);

        std::optional<Intersection> intersect(const Ray& ray, float t_min = 0.001f) const override;
        Vec3 normal_at(Vec3 point) const override;
    };

    struct Circle : public Plane {
        float radius = 1.0f;

        Circle() = default;
        Circle(Vec3 position, Vec3 normal, float radius);

        std::optional<Intersection> intersect(const Ray& ray, float t_min = 0.001f) const override;
        Vec3 random_point(std::mt19937& generator) const;
    };
} // namespace pathtracer
