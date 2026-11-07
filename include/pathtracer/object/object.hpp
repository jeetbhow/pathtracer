#pragma once

#include "pathtracer/geometry/ray.hpp"
#include "pathtracer/geometry/vec3.hpp"
#include "pathtracer/object/material.hpp"

#include <memory>
#include <optional>
#include <numbers>
#include <random>

namespace pathtracer
{
    static constexpr float PI = std::numbers::pi_v<float>;

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

        Material material{};

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

        static Vec3 random_vector(std::mt19937& rng);
    };
} // namespace pathtracer
