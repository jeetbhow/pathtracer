#pragma once

#include "pathtracer/camera.hpp"
#include "pathtracer/image.hpp"
#include "pathtracer/geometry/sphere.hpp"

#include <vector>
#include <cstdint>
#include <random>

namespace pathtracer
{
    struct Scene
    {
        std::vector<Sphere> spheres;
    };

    class Renderer
    {
    public:
        Renderer(const Scene& scene, const Camera& camera, std::uint32_t seed = std::random_device{}())
            : scene_(scene), camera_(camera), generator_(seed)
        {
        }

        Image render() const;
    private:
        Color diffuse_color(const HitRecord& hit, int bounce = 3) const;

        Scene scene_;
        Camera camera_;
        mutable std::mt19937 generator_;
    };
} // namespace pathtracer
