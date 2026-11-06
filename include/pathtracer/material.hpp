#pragma once

#include "pathtracer/geometry/vec3.hpp"

namespace pathtracer
{
    enum class MaterialType
    {
        Diffuse,
    };

    struct Material
    {
        MaterialType type = MaterialType::Diffuse;
        Vec3 albedo{0.5f, 0.5f, 0.5f};
    };
} // namespace pathtracer
