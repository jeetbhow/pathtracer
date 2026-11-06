#include "pathtracer/geometry/sphere.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <cmath>
#include <random>

using std::mt19937;
using std::uniform_real_distribution;
using std::sqrt;

namespace pathtracer
{
    Vec3 Sphere::random_vector(std::mt19937& rng)
    {
        uniform_real_distribution<float> z_dist{-1, 1};
        uniform_real_distribution<float> phi_dist{0.0f, 2.0f * PI};

        float z = z_dist(rng);
        float phi = phi_dist(rng);
        float r = sqrt(1 - z * z);

        return Vec3
        {
            r * static_cast<float>(cos(phi)),
            r * static_cast<float>(sin(phi)),
            z
        };
    }

    Vec3 Sphere::normal(const Vec3 &p) const
    {
        return (p - center_) / radius_;
    }

    std::optional<HitRecord> Sphere::hit(const Ray& ray, float t_min) const
    {
        const Vec3& direction = ray.direction();
        // Promote before arithmetic so small radii survive at large distances.
        const double dx = direction.x;
        const double dy = direction.y;
        const double dz = direction.z;
        const double ox = static_cast<double>(ray.origin().x) - center_.x;
        const double oy = static_cast<double>(ray.origin().y) - center_.y;
        const double oz = static_cast<double>(ray.origin().z) - center_.z;
        const double a = dx * dx + dy * dy + dz * dz;
        const double half_b = ox * dx + oy * dy + oz * dz;
        const double c = ox * ox + oy * oy + oz * oz
                         - static_cast<double>(radius_) * radius_;
        const double discriminant = half_b * half_b - a * c;

        if (a == 0.0f || discriminant < 0.0f)
        {
            return std::nullopt;
        }

        const double root = std::sqrt(discriminant);
        double t = (-half_b - root) / a;

        if (t <= t_min)
        {
            t = (-half_b + root) / a;
            if (t <= t_min)
            {
                return std::nullopt;
            }
        }

        const Vec3 position
        {
            static_cast<float>(ray.origin().x + t * dx),
            static_cast<float>(ray.origin().y + t * dy),
            static_cast<float>(ray.origin().z + t * dz)
        };
        const Vec3 outward_normal = normal(position);
        const Vec3 shading_normal = Vec3::dot(direction, outward_normal) > 0.0f
                                    ? -1.0f * outward_normal : outward_normal;
        return HitRecord{static_cast<float>(t), position, shading_normal, *this, ray};
    }
} // namespace pathtracer
