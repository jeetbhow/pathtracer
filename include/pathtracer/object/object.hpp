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
        Vec3 normal;
        const Material& material; // Non-owning; the material must outlive the hit record.
        Ray incoming;
        bool entering;
    };

    class Object
    {
    public:
        explicit Object(Vec3 position = Vec3{}) : position(position)
        {
        }

        virtual ~Object() = default;

        Vec3 position;
        std::unique_ptr<Material> material = std::make_unique<Diffuse>();

        virtual std::optional<HitRecord> hit(const Ray& ray, float t_min = 0.001f) const = 0;
        virtual Vec3 normal(const Vec3& p) const = 0;
    };

    struct Sphere : public Object
    {
        float radius;

        Sphere(Vec3 position, float radius) : Object(position), radius(radius)
        {
        }

        std::optional<HitRecord> hit(const Ray& ray, float t_min = 0.001f) const override;
        Vec3 normal(const Vec3& p) const override;
    };

    class Plane : public Object
    {
    public:
        Plane(Vec3 position, Vec3 normal);

        std::optional<HitRecord> hit(const Ray& ray, float t_min = 0.001f) const override;
        Vec3 normal(const Vec3& p) const override;

    private:
        Vec3 normal_;
    };
} // namespace pathtracer
