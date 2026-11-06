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

    class Renderer
    {
    public:
        Renderer(const Scene& scene, const Camera& camera)
            : scene_(scene), camera_(camera)
        {
        }

        Image render() const;
    private:
        Scene scene_;
        Camera camera_;
    };
} // namespace pathtracer
