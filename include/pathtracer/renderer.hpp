#pragma once

#include "pathtracer/camera.hpp"
#include "pathtracer/image.hpp"
#include "pathtracer/geometry/sphere.hpp"

#include <vector>

namespace pathtracer
{
    struct Scene
    {
        std::vector<Sphere> spheres;
    };

    Image render(const Camera& camera, const Scene& scene);
} // namespace pathtracer
