#pragma once

#include "pathtracer/geometry/vec3.hpp"

#include <optional>
#include <random>
#include <variant>

namespace pathtracer
{
    struct ShadingContext;

    struct ScatterResult
    {
        Vec3 direction;
        Color attenuation;
    };

    struct Diffuse
    {
        Color albedo;

        explicit Diffuse(const Color& albedo = Color{0.5f, 0.5f, 0.5f}) : albedo(albedo)
        {
        }

        std::optional<ScatterResult> scatter(const ShadingContext& hit, std::mt19937& generator) const;
    };

    struct Metal
    {
        Color albedo;
        float roughness;

        explicit Metal(const Color& albedo = Color{0.5f, 0.5f, 0.5f}, float roughness = 0.0f)
            : albedo(albedo), roughness(roughness)
        {
        }

        std::optional<ScatterResult> scatter(const ShadingContext& hit, std::mt19937& generator) const;
    };

    struct Dielectric
    {
        Color albedo;
        float ior;

        explicit Dielectric(const Color& albedo = Color{1.0f, 1.0f, 1.0f}, float ior = 1.5f)
            : albedo(albedo), ior(ior)
        {
        }

        std::optional<ScatterResult> scatter(const ShadingContext& hit, std::mt19937& generator) const;
    };

    using Material = std::variant<Diffuse, Metal, Dielectric>;

    // Return a unit direction and attenuation, or no result for absorption.
    std::optional<ScatterResult> scatter(const ShadingContext& hit, std::mt19937& generator);
} // namespace pathtracer
