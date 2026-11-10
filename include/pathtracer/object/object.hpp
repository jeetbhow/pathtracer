#pragma once

#include "pathtracer/geometry/plane.hpp"
#include "pathtracer/geometry/sphere.hpp"
#include "pathtracer/object/material.hpp"

#include <memory>
#include <optional>
#include <variant>

namespace pathtracer
{
    struct HitRecord
    {
        float t;
        Vec3 position;
        Vec3 normal;
        const Material& material; // Non-owning; the material must outlive the hit record.
        Ray incoming;
        bool entering;
    };

    using Geometry = std::variant<Sphere, Plane, Circle>;

    class Object
    {
    public:
        Object(Geometry geometry, std::unique_ptr<Material> material);

        std::unique_ptr<Material> material;

        Vec3& position();
        const Vec3& position() const;
        std::optional<HitRecord> hit(const Ray& ray, float t_min = 0.001f) const;
        Vec3 normal_at(Vec3 point) const;

    private:
        Geometry geometry_;
    };
} // namespace pathtracer
