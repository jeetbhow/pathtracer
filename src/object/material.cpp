#include "pathtracer/object/material.hpp"
#include "pathtracer/object/object.hpp"
#include "pathtracer/geometry/sphere.hpp"

#include <random>

using std::mt19937;
using std::nullopt;
using std::optional;
using std::uniform_real_distribution;

namespace pathtracer {
    static const Sphere unit_sphere{Vec3{0, 0, 0}, 1.0f};
    optional<ScatterResult> Diffuse::scatter(const ShadingContext& hit, mt19937& generator) const {
        const Vec3& normal = hit.normal;
        Vec3 direction = normal + unit_sphere.random_point(generator);

        if (length_squared(direction) == 0.0f) {
            direction = normal;
        }

        return ScatterResult{normalize(direction), albedo};
    }

    optional<ScatterResult> Metal::scatter(const ShadingContext& hit, mt19937& generator) const {
        const Ray& incoming = hit.incoming;
        const Vec3& normal = hit.normal;
        Vec3 direction = reflect(normalize(incoming.direction), normal);

        if (roughness > 0.0f) {
            direction += roughness * unit_sphere.random_point(generator);
        }

        if (dot(direction, normal) <= 0.0f) {
            return nullopt;
        }

        return ScatterResult{normalize(direction), albedo};
    }

    optional<ScatterResult> Dielectric::scatter(const ShadingContext& hit, mt19937& generator) const {
        const Ray& incoming = hit.incoming;
        const Vec3& normal = hit.normal;
        const bool entering = hit.entering;
        const float incident_ior = entering ? 1.0f : ior;
        const float transmitted_ior = entering ? ior : 1.0f;
        uniform_real_distribution<float> sample{0.0f, 1.0f};

        const Vec3 direction = refract(
            normalize(incoming.direction),
            normal,
            incident_ior,
            transmitted_ior,
            sample(generator));

        return ScatterResult{normalize(direction), albedo};
    }

    optional<ScatterResult> scatter(const ShadingContext& hit, mt19937& generator) {
        return std::visit(
            [&](const auto& concrete_material) {
                return concrete_material.scatter(hit, generator);
            },
            hit.material);
    }
} // namespace pathtracer
