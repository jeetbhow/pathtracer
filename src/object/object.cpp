#include "pathtracer/object/object.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <utility>
#include <stdexcept>

using std::nullopt;
using std::optional;

namespace pathtracer {
    Object::Object(std::unique_ptr<Geometry> geometry, Material material)
        : geometry(std::move(geometry)), material(std::move(material)) {
        if (!this->geometry) {
            throw std::invalid_argument("Object geometry must not be null");
        }
    }

    Vec3& Object::position() {
        return geometry->position;
    }

    const Vec3& Object::position() const {
        return geometry->position;
    }

    Vec3 Object::normal_at(Vec3 point) const {
        return geometry->normal_at(point);
    }

    optional<ShadingContext> Object::hit(const Ray& ray, float t_min) const {
        const auto intersection = geometry->intersect(ray, t_min);

        if (!intersection) {
            return nullopt;
        }

        const Vec3 outward_normal = intersection->outward_normal;
        const float outward_normal_dot = dot(ray.direction, outward_normal);
        const Vec3 shading_normal = outward_normal_dot > 0.0f ? -1.0f * outward_normal : outward_normal;

        return ShadingContext{
            intersection->t,
            intersection->position,
            shading_normal,
            material,
            ray,
            outward_normal_dot < 0.0f};
    }
} // namespace pathtracer
