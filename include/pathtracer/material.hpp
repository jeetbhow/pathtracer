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
        Vec3 albedo{1.0f, 1.0f, 1.0f};
    };
} // namespace pathtracer
