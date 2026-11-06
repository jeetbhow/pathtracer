#pragma once

#include "pathtracer/geometry/ray.hpp"
#include "pathtracer/geometry/hit_record.hpp"
#include "pathtracer/geometry/vec3.hpp"
#include "pathtracer/material.hpp"

#include <numbers>
#include <optional>
#include <random>

namespace pathtracer
{
    static constexpr float PI = std::numbers::pi_v<float>;

    class Sphere
    {
    public:
        Material material{};

        Sphere(const Vec3& center, float radius) : radius_(radius), center_(center)
        {
        }

        std::optional<HitRecord> hit(const Ray& ray) const;
        Vec3 normal(const Vec3& p) const;
        
        static Vec3 random_vector(std::mt19937& rng);

    private:
        float radius_;
        Vec3 center_;
    };
} // namespace pathtracer
