#pragma once

#include "pathtracer/camera.hpp"
#include "pathtracer/image.hpp"
#include "pathtracer/geometry/sphere.hpp"

namespace pathtracer
{

    Color skybox_color(const Ray& r);
    Color surface_normal(const Ray& r, float t, const Sphere& s);
    Image render(const Camera& cam, const Sphere& s);

} // namespace pathtracer
