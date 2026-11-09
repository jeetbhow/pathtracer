#pragma once

namespace pathtracer
{
    struct Vec3
    {
        float x;
        float y;
        float z;

        static float dot(const Vec3& a, const Vec3& b);
        // Right-handed cross product; inputs need not be unit vectors.
        static Vec3 cross(const Vec3& a, const Vec3& b);
        static Vec3 proj_unit(const Vec3& a, const Vec3& b);
        static Vec3 reflect(const Vec3& incoming, const Vec3& normal);
        // Unit I and N, with N opposing I; reflection_sample is uniform in [0, 1).
        static Vec3 refract(const Vec3& I, const Vec3& N, float n_a, float n_b, float reflection_sample);

        float length_squared() const;
        float length() const;
        Vec3 normalized() const;
    };

    using Color = Vec3;

    static inline Color tint(const Color& color, const Color& tint_color)
    {
        return Color{color.x * tint_color.x, color.y * tint_color.y, color.z * tint_color.z};
    }

    Vec3 operator+(const Vec3& a, const Vec3& b);
    Vec3& operator+=(Vec3& a, const Vec3& b);
    Vec3 operator-(const Vec3& a, const Vec3& b);
    Vec3 operator*(const Vec3& a, float scalar);
    Vec3 operator*(float scalar, const Vec3& a);
    Vec3 operator/(const Vec3& a, float scalar);
} // namespace pathtracer
