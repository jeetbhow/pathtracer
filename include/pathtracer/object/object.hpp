#pragma once

#include "pathtracer/geometry/plane.hpp"
#include "pathtracer/geometry/sphere.hpp"
#include "pathtracer/object/material.hpp"

#include <optional>
#include <variant>

namespace pathtracer
{
    struct ShadingContext
    {
        float t;
        Vec3 position;
        Vec3 normal;
        const Material& material; // Non-owning; the material must outlive the shading context.
        Ray incoming;
        bool entering;
    };

    using Geometry = std::variant<Sphere, Plane, Circle>;

    struct Object
    {
        Object(Geometry geometry, Material material);

        Geometry geometry;
        Material material;

        Vec3& position();
        const Vec3& position() const;
        std::optional<ShadingContext> hit(const Ray& ray, float t_min = 0.001f) const;
        Vec3 normal_at(Vec3 point) const;
    };
} // namespace pathtracer
