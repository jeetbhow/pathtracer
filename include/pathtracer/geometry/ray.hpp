#pragma once

#include "pathtracer/geometry/vec3.hpp"

#include <string>

namespace pathtracer
{
    class Ray
    {
    public:
        Ray(const Vec3& origin, const Vec3& direction) : origin_(origin), direction_(direction)
        {
        }

        const Vec3 &direction() const
        {
            return direction_;
        }
        const Vec3 &origin() const
        {
            return origin_;
        }

        Vec3 at(float t) const;

        std::string to_string() const;

    private:
        Vec3 origin_;
        Vec3 direction_;
    };
} // namespace pathtracer
