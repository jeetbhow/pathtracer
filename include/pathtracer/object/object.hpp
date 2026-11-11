#pragma once

#include "pathtracer/geometry/geometry.hpp"
#include "pathtracer/object/material.hpp"

#include <optional>
#include <memory>

namespace pathtracer {
    struct ShadingContext {
        float t;
        Vec3 position;
        Vec3 normal;
        const Material& material; // Non-owning; the material must outlive the shading context.
        Ray incoming;
        bool entering;
    };

    struct Object {
        Object(std::unique_ptr<Geometry> geometry, Material material);

        std::unique_ptr<Geometry> geometry;
        Material material;

        Vec3& position();
        const Vec3& position() const;
        std::optional<ShadingContext> hit(const Ray& ray, float t_min = 0.001f) const;
        Vec3 normal_at(Vec3 point) const;
    };
} // namespace pathtracer
