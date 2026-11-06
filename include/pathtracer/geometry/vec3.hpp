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

    using Color = Vec3;

    static inline Color apply_albedo(const Color& light, const Color& albedo)
    {
        return Color{light.x * albedo.x, light.y * albedo.y, light.z * albedo.z};
    }

    Vec3 operator+(const Vec3& a, const Vec3& b);
    Vec3& operator+=(Vec3& a, const Vec3& b);
    Vec3 operator-(const Vec3& a, const Vec3& b);
    Vec3 operator*(const Vec3& a, float scalar);
    Vec3 operator*(float scalar, const Vec3& a);
    Vec3 operator/(const Vec3& a, float scalar);
} // namespace pathtracer
