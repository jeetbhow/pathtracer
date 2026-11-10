#include "pathtracer/object/material.hpp"
#include "pathtracer/geometry/sphere.hpp"

#include <random>

using std::mt19937;
using std::nullopt;
using std::optional;
using std::uniform_real_distribution;

namespace pathtracer
{
    static constexpr Sphere unit_sphere{Vec3{0, 0, 0}, 1.0f};

    optional<ScatterResult> Diffuse::scatter(const Ray&, const Vec3& normal,
                                                 bool, mt19937& generator) const
    {
        Vec3 direction = normal + unit_sphere.random_point(generator);
        if (length_squared(direction) == 0.0f)
        {
            direction = normal;
        }
        return ScatterResult{normalize(direction), albedo};
    }

    optional<ScatterResult> Metal::scatter(const Ray& incoming, const Vec3& normal,
                                               bool, mt19937& generator) const
    {
        Vec3 direction = reflect(normalize(incoming.direction), normal);
        if (roughness > 0.0f)
        {
            direction += roughness * unit_sphere.random_point(generator);
        }
        if (dot(direction, normal) <= 0.0f)
        {
            return nullopt;
        }
        return ScatterResult{normalize(direction), albedo};
    }

    optional<ScatterResult> Dielectric::scatter(const Ray& incoming, const Vec3& normal,
                                                    bool entering, mt19937& generator) const
    {
        const float incident_ior = entering ? 1.0f : ior;
        const float transmitted_ior = entering ? ior : 1.0f;
        uniform_real_distribution<float> sample{0.0f, 1.0f};
        const Vec3 direction = refract(normalize(incoming.direction), normal,
                                            incident_ior, transmitted_ior, sample(generator));
        return ScatterResult{normalize(direction), albedo};
    }

    optional<ScatterResult> scatter(const Material& material, const Ray& incoming,
                                   const Vec3& normal, bool entering, mt19937& generator)
    {
        return std::visit([&](const auto& concrete_material) {
            return concrete_material.scatter(incoming, normal, entering, generator);
        }, material);
    }
} // namespace pathtracer
