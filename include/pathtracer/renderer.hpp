#pragma once

#include "pathtracer/camera.hpp"
#include "pathtracer/image.hpp"
#include "pathtracer/geometry/sphere.hpp"

namespace pathtracer
{
    Image render(const Camera& camera, const Sphere& sphere);
} // namespace pathtracer
