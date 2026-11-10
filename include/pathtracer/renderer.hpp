#pragma once

#include "pathtracer/camera/camera.hpp"
#include "pathtracer/camera/image.hpp"
#include "pathtracer/object/object.hpp"

#include <vector>
#include <cstdint>
#include <random>

namespace pathtracer
{
    struct Scene
    {
        // Hit records borrow materials from these objects and require stable storage.
        std::vector<Object> objects;
    };

    class Renderer
    {
    public:
        // Non-owning: scene and camera must outlive this Renderer and any copies.
        Renderer(const Scene& scene, const Camera& camera, std::uint32_t seed = std::random_device{}())
            : scene_(scene), camera_(camera), generator_(seed)
        {
        }

        Renderer(const Scene&&, const Camera&, std::uint32_t = std::random_device{}()) = delete;
        Renderer(const Scene&, const Camera&&, std::uint32_t = std::random_device{}()) = delete;

        Image& render() const;
    private:
        Color handle_hit(const HitRecord& hit) const;

        const int max_bounces_ = 8;
        mutable int bounces_remaining_ = max_bounces_;
        const Scene& scene_;
        const Camera& camera_;
        mutable std::mt19937 generator_;
    };
} // namespace pathtracer
