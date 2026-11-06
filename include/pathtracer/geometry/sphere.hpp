#pragma once

#include "pathtracer/geometry/ray.hpp"

#include <optional>

namespace pathtracer
{
    class Sphere
    {
    public:
        Sphere(const Vec3& center, float radius) : radius_(radius), center_(center)
        {
        }

        std::optional<float> hit(const Ray& ray) const;
        constexpr Vec3 normal(const Vec3& p) const
        {
            return (p - center_) / radius_;
        }
    private:
        float radius_;
        Vec3 center_;
    };
} // namespace pathtracer
