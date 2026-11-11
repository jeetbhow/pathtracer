#include "pathtracer/object/object.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <utility>

using std::nullopt;
using std::optional;

namespace pathtracer
{
    Object::Object(Geometry geometry, Material material)
        : geometry(std::move(geometry)), material(std::move(material))
    {
    }

    Vec3& Object::position()
    {
        return std::visit([](auto& shape) -> Vec3& { return shape.position; }, geometry);
    }

    const Vec3& Object::position() const
    {
        return std::visit([](const auto& shape) -> const Vec3& { return shape.position; }, geometry);
    }

    Vec3 Object::normal_at(Vec3 point) const
    {
        return std::visit([point](const auto& shape) { return shape.normal_at(point); }, geometry);
    }

    optional<ShadingContext> Object::hit(const Ray& ray, float t_min) const
    {
        const auto intersection = std::visit([&ray, t_min](const auto& shape) {
            return shape.intersect(ray, t_min);
        }, geometry);
        if (!intersection)
        {
            return nullopt;
        }

        const Vec3 outward_normal = intersection->outward_normal;
        const float outward_normal_dot = dot(ray.direction, outward_normal);
        const Vec3 shading_normal = outward_normal_dot > 0.0f ? -1.0f * outward_normal : outward_normal;

        return ShadingContext
        {
            intersection->t,
            intersection->position,
            shading_normal,
            material,
            ray,
            outward_normal_dot < 0.0f
        };
    }
} // namespace pathtracer
