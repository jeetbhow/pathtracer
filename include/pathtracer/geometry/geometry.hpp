#pragma once

#include "pathtracer/geometry/intersection.hpp"
#include "pathtracer/geometry/ray.hpp"

#include <optional>

namespace pathtracer
{
    struct Geometry
    {
        Vec3 position{0, 0, 0};

        Geometry() = default;
        explicit Geometry(Vec3 position) : position(position) {}
        virtual ~Geometry() = default;

        virtual std::optional<Intersection> intersect(const Ray& ray, float t_min = 0.001f) const = 0;
        virtual Vec3 normal_at(Vec3 point) const = 0;
    };
} // namespace pathtracer
