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

    std::optional<HitRecord> Sphere::hit(const Ray& ray) const
    {
        const Vec3 offset = ray.origin() - center_;
        const Vec3& direction = ray.direction();
        const float a = direction.length_squared();
        const float half_b = Vec3::dot(offset, direction);
        const float c = offset.length_squared() - radius_ * radius_;
        const float discriminant = half_b * half_b - a * c;

        if (a == 0.0f || discriminant < 0.0f)
        {
            return std::nullopt;
        }

        const float root = std::sqrt(discriminant);
        const float near = (-half_b - root) / a;

        if (near > 0.0f)
        {
            const Vec3 position = ray.at(near);
            return HitRecord{near, position, normal(position), *this};
        }

        const float far = (-half_b + root) / a;
        if (far > 0.0f)
        {
            const Vec3 position = ray.at(far);
            return HitRecord{far, position, normal(position), *this};
        }

        return std::nullopt;
    }
} // namespace pathtracer
