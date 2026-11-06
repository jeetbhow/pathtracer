#include "pathtracer/geometry/sphere.hpp"

#include <cmath>

namespace pathtracer
{
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
            return HitRecord{near, position, normal(position)};
        }

        const float far = (-half_b + root) / a;
        if (far > 0.0f)
        {
            const Vec3 position = ray.at(far);
            return HitRecord{far, position, normal(position)};
        }

        return std::nullopt;
    }
} // namespace pathtracer
