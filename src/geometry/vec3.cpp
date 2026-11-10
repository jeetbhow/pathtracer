#include "pathtracer/geometry/vec3.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>

using std::clamp;
using std::pow;
using std::sqrt;

namespace pathtracer
{
    float to_radians(float degrees)
    {
        return degrees * std::numbers::pi_v<float> / 180.0f;
    }

    float dot(Vec3 a, Vec3 b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vec3 cross(Vec3 a, Vec3 b)
    {
        return Vec3{
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x,
        };
    }

    // Assume a and b are unit vectors. Project a onto b.
    Vec3 proj_unit(Vec3 a, Vec3 b)
    {
        return dot(a, b) * b;
    }

    Vec3 reflect(Vec3 incoming, Vec3 normal)
    {
        return incoming - (2 * dot(incoming, normal)) * normal;
    }

    Vec3 refract(Vec3 I, Vec3 N, float n_a, float n_b, float reflection_sample)
    {
        if (n_a == n_b)
        {
            return I;
        }

        float eta = n_a / n_b;

        Vec3 I_y = proj_unit(I, N);
        Vec3 I_x = I - I_y;
        Vec3 refrac_x = eta * I_x;

        float k = 1 - dot(refrac_x, refrac_x);
        if (k < 0) {
            return reflect(I, N);
        }

        const float cos_incident = clamp(-dot(I, N), 0.0f, 1.0f);
        const float r = (n_a - n_b) / (n_a + n_b);
        const float reflectance = r * r + (1.0f - r * r) * pow(1.0f - cos_incident, 5.0f);
        if (reflection_sample < reflectance)
        {
            return reflect(I, N);
        }

        Vec3 refrac_y = -sqrt(k) * N;
        return refrac_x + refrac_y;
    }

    float length_squared(Vec3 v)
    {
        return dot(v, v);
    }

    float length(Vec3 v)
    {
        return sqrt(length_squared(v));
    }

    Vec3 normalize(Vec3 v)
    {
        const float len = length(v);
        return len == 0.0f ? Vec3{0.0f, 0.0f, 0.0f} : v / len;
    }

    Color tint(Color color, Color tint_color)
    {
        return Color{color.x * tint_color.x, color.y * tint_color.y, color.z * tint_color.z};
    }

    Vec3 operator+(Vec3 a, Vec3 b)
    {
        return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
    }

    Vec3& operator+=(Vec3& a, Vec3 b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

    Vec3 operator-(Vec3 a, Vec3 b)
    {
        return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
    }

    Vec3 operator*(Vec3 a, float scalar)
    {
        return Vec3{a.x * scalar, a.y * scalar, a.z * scalar};
    }

    Vec3 operator*(float scalar, Vec3 a)
    {
        return a * scalar;
    }

    Vec3 operator/(Vec3 a, float scalar)
    {
        return a * (1.0f / scalar);
    }

} // namespace pathtracer
