#include "pathtracer/object/object.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <cmath>
#include <stdexcept>

using std::nullopt;
using std::optional;
using std::sqrt;

namespace pathtracer
{
    Plane::Plane(Vec3 position, Vec3 normal) : Object(position), normal_(normalize(normal))
    {
        if (length_squared(normal_) == 0.0f)
        {
            throw std::invalid_argument("Plane normal must be nonzero");
        }
    }

    Vec3 Plane::normal(const Vec3&) const
    {
        return normal_;
    }

    optional<HitRecord> Plane::hit(const Ray& ray, float t_min) const
    {
        const float denominator = dot(normal_, ray.direction);
        if (denominator == 0.0f)
        {
            return nullopt;
        }

        const float t = dot(position - ray.origin, normal_) / denominator;
        if (t <= t_min)
        {
            return nullopt;
        }

        const bool entering = denominator < 0.0f;
        return HitRecord{
            t,
            ray.at(t),
            entering ? normal_ : -1.0f * normal_,
            *material,
            ray,
            entering,
        };
    }

    Vec3 Sphere::normal(const Vec3 &p) const
    {
        return (p - position) / radius;
    }

    optional<HitRecord> Sphere::hit(const Ray& ray, float t_min) const
    {
        const Vec3& direction = ray.direction;
        // Promote before arithmetic so small radii survive at large distances.
        const double dx = direction.x;
        const double dy = direction.y;
        const double dz = direction.z;
        const double ox = static_cast<double>(ray.origin.x) - position.x;
        const double oy = static_cast<double>(ray.origin.y) - position.y;
        const double oz = static_cast<double>(ray.origin.z) - position.z;
        const double a = dx * dx + dy * dy + dz * dz;
        const double half_b = ox * dx + oy * dy + oz * dz;
        const double c = ox * ox + oy * oy + oz * oz
                         - static_cast<double>(radius) * radius;
        const double discriminant = half_b * half_b - a * c;

        if (a == 0.0f || discriminant < 0.0f)
        {
            return nullopt;
        }

        const double root = sqrt(discriminant);
        double t = (-half_b - root) / a;

        if (t <= t_min)
        {
            t = (-half_b + root) / a;
            if (t <= t_min)
            {
                return nullopt;
            }
        }

        const Vec3 hit_position
        {
            static_cast<float>(ray.origin.x + t * dx),
            static_cast<float>(ray.origin.y + t * dy),
            static_cast<float>(ray.origin.z + t * dz)
        };

        Vec3 outward_normal = normal(hit_position);
        const float outward_normal_dot = dot(direction, outward_normal);

        bool entering = outward_normal_dot < 0.0f ? true : false;
        outward_normal = outward_normal_dot > 0.0f ? -1.0f * outward_normal : outward_normal;

        return HitRecord
        {
            static_cast<float>(t),
            hit_position,
            outward_normal,
            *material,
            ray,
            entering
        };
    }
} // namespace pathtracer
