#pragma once

namespace pathtracer
{
    float to_radians(float degrees);

    struct Vec3
    {
        float x;
        float y;
        float z;
    };

    Vec3 operator+(Vec3 a, Vec3 b);
    Vec3& operator+=(Vec3& a, Vec3 b);
    Vec3 operator-(Vec3 a, Vec3 b);
    Vec3 operator*(Vec3 a, float scalar);
    Vec3 operator*(float scalar, Vec3 a);
    Vec3 operator/(Vec3 a, float scalar);

    float dot(Vec3 a, Vec3 b);
    Vec3 cross(Vec3 a, Vec3 b);
    Vec3 proj_unit(Vec3 a, Vec3 b);
    Vec3 reflect(Vec3 I, Vec3 N);
    Vec3 refract(Vec3 I, Vec3 N, float n_a, float n_b, float sample);
    float length_squared(Vec3 v);
    float length(Vec3 v);
    Vec3 normalize(Vec3 v);

    using Color = Vec3;

    Color tint(Color color, Color tint_color);
} // namespace pathtracer
