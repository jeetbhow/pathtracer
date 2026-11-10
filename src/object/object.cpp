#include "pathtracer/object/object.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <stdexcept>
#include <utility>

using std::nullopt;
using std::optional;

namespace pathtracer
{
    Object::Object(Geometry geometry, std::unique_ptr<Material> material)
        : material(std::move(material)), geometry_(std::move(geometry))
    {
        if (!this->material)
        {
            throw std::invalid_argument("Object requires a material");
        }
    }

    Vec3& Object::position()
    {
        return std::visit([](auto& shape) -> Vec3& { return shape.position; }, geometry_);
    }

    const Vec3& Object::position() const
    {
        return std::visit([](const auto& shape) -> const Vec3& { return shape.position; }, geometry_);
    }

    Vec3 Object::normal_at(Vec3 point) const
    {
        return std::visit([point](const auto& shape) { return shape.normal_at(point); }, geometry_);
    }

    optional<HitRecord> Object::hit(const Ray& ray, float t_min) const
    {
        const auto intersection = std::visit([&ray, t_min](const auto& shape) {
            return shape.intersect(ray, t_min);
        }, geometry_);
        if (!intersection)
        {
            return nullopt;
        }

        const Vec3 outward_normal = intersection->outward_normal;
        const float outward_normal_dot = dot(ray.direction, outward_normal);
        const Vec3 shading_normal = outward_normal_dot > 0.0f ? -1.0f * outward_normal : outward_normal;

        return HitRecord
        {
            intersection->t,
            intersection->position,
            shading_normal,
            *material,
            ray,
            outward_normal_dot < 0.0f
        };
    }
} // namespace pathtracer
