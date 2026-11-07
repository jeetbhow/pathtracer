#pragma once

#include "pathtracer/geometry/vec3.hpp"

#include <string>

namespace pathtracer
{
    struct Ray
    {
        Vec3 origin;
        Vec3 direction;

        Vec3 at(float t) const;

        std::string to_string() const;
    };
} // namespace pathtracer
