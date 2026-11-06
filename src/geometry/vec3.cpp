#include "pathtracer/geometry/vec3.hpp"

#include <cmath>

namespace pathtracer
{

    float Vec3::dot(const Vec3& a, const Vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    float Vec3::length_squared() const
    {
        return dot(*this, *this);
    }

    float Vec3::length() const
    {
        return std::sqrt(length_squared());
    }

    Vec3 Vec3::normalized() const
    {
        const float len = length();
        return len == 0.0f ? Vec3{0.0f, 0.0f, 0.0f} : *this / len;
    }

} // namespace pathtracer
