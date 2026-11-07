#pragma once

#include "pathtracer/geometry/vec3.hpp"

namespace pathtracer
{
    enum class MaterialType
    {
        Diffuse,
        Metal,
        Dielectric,
    };

    struct Material
    {
        MaterialType type = MaterialType::Diffuse;
        Vec3 albedo{0.5f, 0.5f, 0.5f};
        float roughness = 0.0f;
        float ior = 1.5f;
    };
} // namespace pathtracer
