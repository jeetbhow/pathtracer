#include "pathtracer/geometry/sphere.hpp"

#include <cmath>
#include <numbers>

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
    Vec3 Sphere::normal_at(Vec3 point) const
    {
        return (point - position) / radius;
    }

    Vec3 Sphere::random_point(mt19937& generator) const
    {
        uniform_real_distribution<float> z_dist{-1, 1};
        uniform_real_distribution<float> phi_dist{0.0f, 2.0f * pi_v<float>};

        const float z = z_dist(generator);
        const float phi = phi_dist(generator);
        const float r = sqrt(1 - z * z);

        return position + radius * Vec3{
            r * cos(phi),
            r * sin(phi),
            z,
        };
    }

    optional<Intersection> Sphere::intersect(const Ray& ray, float t_min) const
    {
        // Promote before arithmetic so small radii survive at large distances.
        const double dx = ray.direction.x;
        const double dy = ray.direction.y;
        const double dz = ray.direction.z;
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

        const Vec3 hit_position{
            static_cast<float>(ray.origin.x + t * dx),
            static_cast<float>(ray.origin.y + t * dy),
            static_cast<float>(ray.origin.z + t * dz),
        };

        return Intersection{static_cast<float>(t), hit_position, normal_at(hit_position)};
    }
} // namespace pathtracer
