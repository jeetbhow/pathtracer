#pragma once

#include "pathtracer/geometry/ray.hpp"
#include "pathtracer/geometry/vec3.hpp"
#include "pathtracer/object/material.hpp"

#include <memory>
#include <optional>

namespace pathtracer
{
    struct HitRecord
    {
        float t;
        Vec3 position;
        Vec3 normal; // Shading normal, oriented against the incoming ray.
        const Material& material; // Non-owning; the material must outlive the hit record.
        Ray incoming;
        bool entering;
    };

    class Object
    {
    public:
        virtual ~Object() = default;

        std::shared_ptr<const Material> material = std::make_shared<Diffuse>();

        virtual std::optional<HitRecord> hit(const Ray& ray, float t_min = 0.001f) const = 0;
        virtual Vec3 normal(const Vec3& p) const = 0;
    };

    struct Sphere : public Object
    {
        Vec3 center;
        float radius;

        Sphere(const Vec3& center, float radius) : center(center), radius(radius)
        {
        }

        std::optional<HitRecord> hit(const Ray& ray, float t_min = 0.001f) const override;
        Vec3 normal(const Vec3& p) const override;
    };
} // namespace pathtracer
