#include "pathtracer/geometry/plane.hpp"

#include <cmath>
#include <numbers>
#include <stdexcept>

using std::cos;
using std::mt19937;
using std::nullopt;
using std::numbers::pi_v;
using std::optional;
using std::sin;
using std::sqrt;
using std::uniform_real_distribution;

namespace pathtracer
{
    Plane::Plane(Vec3 position, Vec3 normal) : position(position), normal(normal)
    {
        if (length_squared(normal) == 0.0f)
        {
            throw std::invalid_argument("Plane normal must be nonzero");
        }
    }

    Vec3 Plane::normal_at(Vec3) const
    {
        return normal;
    }

    optional<Intersection> Plane::intersect(const Ray& ray, float t_min) const
    {
        const float denominator = dot(normal, ray.direction);
        if (denominator == 0.0f)
        {
            return nullopt;
        }

        const float t = dot(position - ray.origin, normal) / denominator;
        if (t <= t_min)
        {
            return nullopt;
        }

        return Intersection{t, ray.at(t), normal};
    }

    Circle::Circle(Vec3 position, Vec3 normal, float radius)
        : Plane(position, normal), radius(radius)
    {
        if (!std::isfinite(radius) || radius <= 0.0f)
        {
            throw std::invalid_argument("Circle radius must be positive and finite");
        }
    }

    optional<Intersection> Circle::intersect(const Ray& ray, float t_min) const
    {
        const auto intersection = Plane::intersect(ray, t_min);
        if (intersection && length_squared(intersection->position - position) > radius * radius)
        {
            return nullopt;
        }
        return intersection;
    }

    Vec3 Circle::random_point(mt19937& rng) const
    {
        uniform_real_distribution<float> distribution{0.0f, 1.0f};
        const float r = radius * sqrt(distribution(rng));
        const float theta = 2.0f * pi_v<float> * distribution(rng);

        const Vec3 helper = std::abs(normal.y) < 0.9f
            ? Vec3{0, 1, 0}
            : Vec3{1, 0, 0};

        const Vec3 tangent = normalize(cross(normal, helper));
        const Vec3 bitangent = cross(normal, tangent);

        return position + (r * cos(theta)) * tangent + (r * sin(theta)) * bitangent;
    }
} // namespace pathtracer
