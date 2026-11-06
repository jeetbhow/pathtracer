#pragma once

namespace pathtracer
{
    struct Vec3
    {
        float x;
        float y;
        float z;

        static float dot(const Vec3& a, const Vec3& b);
        float length_squared() const;
        float length() const;
        Vec3 normalized() const;
    };

    constexpr Vec3 operator+(const Vec3& a, const Vec3& b)
    {
        return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
    }
    constexpr Vec3 operator-(const Vec3& a, const Vec3& b)
    {
        return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
    }
    constexpr Vec3 operator*(const Vec3& a, float scalar)
    {
        return Vec3{a.x * scalar, a.y * scalar, a.z * scalar};
    }
    constexpr Vec3 operator*(float scalar, const Vec3& a)
    {
        return a * scalar;
    }
    constexpr Vec3 operator/(const Vec3& a, float scalar)
    {
        return a * (1.0f / scalar);
    }
} // namespace pathtracer
