#include "pathtracer/object/material.hpp"

#include <cmath>
#include <numbers>
#include <random>

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
    static Vec3 random_unit_vector(mt19937& rng)
    {
        uniform_real_distribution<float> z_dist{-1, 1};
        uniform_real_distribution<float> phi_dist{0.0f, 2.0f * pi_v<float>};

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

    optional<ScatterResult> Diffuse::scatter(const Ray&, const Vec3& normal,
                                                 bool, mt19937& generator) const
    {
        Vec3 direction = normal + random_unit_vector(generator);
        if (direction.length_squared() == 0.0f)
        {
            direction = normal;
        }
        return ScatterResult{direction.normalized(), albedo};
    }

    optional<ScatterResult> Metal::scatter(const Ray& incoming, const Vec3& normal,
                                               bool, mt19937& generator) const
    {
        Vec3 direction = Vec3::reflect(incoming.direction.normalized(), normal);
        if (roughness > 0.0f)
        {
            direction += roughness * random_unit_vector(generator);
        }
        if (Vec3::dot(direction, normal) <= 0.0f)
        {
            return nullopt;
        }
        return ScatterResult{direction.normalized(), albedo};
    }

    optional<ScatterResult> Dielectric::scatter(const Ray& incoming, const Vec3& normal,
                                                    bool entering, mt19937& generator) const
    {
        const float incident_ior = entering ? 1.0f : ior;
        const float transmitted_ior = entering ? ior : 1.0f;
        uniform_real_distribution<float> sample{0.0f, 1.0f};
        const Vec3 direction = Vec3::refract(incoming.direction.normalized(), normal,
                                            incident_ior, transmitted_ior, sample(generator));
        return ScatterResult{direction.normalized(), albedo};
    }
} // namespace pathtracer
