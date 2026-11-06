#pragma once

#include "pathtracer/camera.hpp"
#include "pathtracer/image.hpp"
#include "pathtracer/geometry/sphere.hpp"

namespace pathtracer
{

    Color skybox_color(const Ray& ray);
    Image render(const Camera& camera, const Sphere& sphere);

} // namespace pathtracer
