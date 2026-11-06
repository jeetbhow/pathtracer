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

        const auto make_hit = [&](float t)
        {
            const Vec3 position = ray.at(t);
            const Vec3 outward_normal = normal(position);
            const Vec3 shading_normal = Vec3::dot(direction, outward_normal) > 0.0f
                                        ? -1.0f * outward_normal : outward_normal;
            return HitRecord{t, position, shading_normal, *this, ray};
        };

        const float root = std::sqrt(discriminant);
        const float near = (-half_b - root) / a;

        if (near > t_min)
        {
            return make_hit(near);
        }

        const float far = (-half_b + root) / a;
        if (far > t_min)
        {
            return make_hit(far);
        }

        return std::nullopt;
    }
} // namespace pathtracer
