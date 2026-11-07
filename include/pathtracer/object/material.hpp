#pragma once

#include "pathtracer/geometry/ray.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <optional>
#include <random>

namespace pathtracer
{
    struct ScatterResult
    {
        Vec3 direction;
        Color attenuation;
    };

    class Material
    {
    public:
        virtual ~Material() = default;

        // Return a unit scatter direction and color attenuation, or no result for absorption.
        virtual std::optional<ScatterResult> scatter(const Ray& incoming, const Vec3& normal,
                                                    bool entering, std::mt19937& generator) const = 0;
    };

    struct Diffuse : public Material
    {
        Color albedo;

        explicit Diffuse(const Color& albedo = Color{0.5f, 0.5f, 0.5f}) : albedo(albedo)
        {
        }

        std::optional<ScatterResult> scatter(const Ray& incoming, const Vec3& normal,
                                            bool entering, std::mt19937& generator) const override;
    };

    struct Metal : public Material
    {
        Color albedo;
        float roughness;

        explicit Metal(const Color& albedo = Color{0.5f, 0.5f, 0.5f}, float roughness = 0.0f)
            : albedo(albedo), roughness(roughness)
        {
        }

        std::optional<ScatterResult> scatter(const Ray& incoming, const Vec3& normal,
                                            bool entering, std::mt19937& generator) const override;
    };

    struct Dielectric : public Material
    {
        Color albedo;
        float ior;

        explicit Dielectric(const Color& albedo = Color{1.0f, 1.0f, 1.0f}, float ior = 1.5f)
            : albedo(albedo), ior(ior)
        {
        }

        std::optional<ScatterResult> scatter(const Ray& incoming, const Vec3& normal,
                                            bool entering, std::mt19937& generator) const override;
    };
} // namespace pathtracer
