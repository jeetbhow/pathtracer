#pragma once

#include "pathtracer/geometry/ray.hpp"
#include "pathtracer/geometry/hit_record.hpp"

#include <optional>

namespace pathtracer
{
    class Sphere
    {
    public:
        Sphere(const Vec3& center, float radius) : radius_(radius), center_(center)
        {
        }

        std::optional<HitRecord> hit(const Ray& ray) const;
        Vec3 normal(const Vec3& p) const;
    private:
        float radius_;
        Vec3 center_;
    };
} // namespace pathtracer
