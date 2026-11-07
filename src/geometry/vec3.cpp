#include "pathtracer/geometry/vec3.hpp"

#include <algorithm>
#include <cmath>

namespace pathtracer
{
    float Vec3::dot(const Vec3& a, const Vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    // Assume a and b are unit vectors. Project a onto b.
    Vec3 Vec3::proj_unit(const Vec3 &a, const Vec3 &b)
    {
        return Vec3::dot(a, b) * b;
    }

    Vec3 Vec3::reflect(const Vec3 &incoming, const Vec3 &normal)
    {
        return incoming - (2 * Vec3::dot(incoming, normal)) * normal;
    }

    Vec3 Vec3::refract(const Vec3& I, const Vec3& N, float n_a, float n_b, float reflection_sample)
    {
        if (n_a == n_b)
        {
            return I;
        }

        float eta = n_a / n_b;

        Vec3 I_y = Vec3::proj_unit(I, N);
        Vec3 I_x = I - I_y;
        Vec3 refrac_x = eta * I_x;

        float k = 1 - Vec3::dot(refrac_x, refrac_x);
        if (k < 0) {
            return reflect(I, N);
        }

        const float cos_incident = std::clamp(-Vec3::dot(I, N), 0.0f, 1.0f);
        const float r = (n_a - n_b) / (n_a + n_b);
        const float reflectance = r * r + (1.0f - r * r) * std::pow(1.0f - cos_incident, 5.0f);
        if (reflection_sample < reflectance)
        {
            return reflect(I, N);
        }

        Vec3 refrac_y = -sqrt(k) * N;
        return refrac_x + refrac_y;
    }

    float Vec3::length_squared() const
    {
        return dot(*this, *this);
    }

    float Vec3::length() const
    {
        return sqrt(length_squared());
    }

    Vec3 Vec3::normalized() const
    {
        const float len = length();
        return len == 0.0f ? Vec3{0.0f, 0.0f, 0.0f} : *this / len;
    }

    Vec3 operator+(const Vec3& a, const Vec3& b)
    {
        return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
    }

    Vec3& operator+=(Vec3& a, const Vec3& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

    Vec3 operator-(const Vec3& a, const Vec3& b)
    {
        return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
    }

    Vec3 operator*(const Vec3& a, float scalar)
    {
        return Vec3{a.x * scalar, a.y * scalar, a.z * scalar};
    }

    Vec3 operator*(float scalar, const Vec3& a)
    {
        return a * scalar;
    }

    Vec3 operator/(const Vec3& a, float scalar)
    {
        return a * (1.0f / scalar);
    }

} // namespace pathtracer
