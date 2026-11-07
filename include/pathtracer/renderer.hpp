#pragma once

#include "pathtracer/camera/camera.hpp"
#include "pathtracer/camera/image.hpp"
#include "pathtracer/object/object.hpp"

#include <vector>
#include <memory>
#include <cstdint>
#include <random>

namespace pathtracer
{
    struct Scene
    {
        std::vector<std::unique_ptr<Object>> objects;
    };

    class Renderer
    {
    public:
        // Non-owning: scene must outlive this Renderer and any copies of it.
        Renderer(const Scene& scene, const Camera& camera, std::uint32_t seed = std::random_device{}())
            : scene_(scene), camera_(camera), generator_(seed)
        {
        }

        Renderer(const Scene&&, const Camera&, std::uint32_t = std::random_device{}()) = delete;

        Image& render() const;
    private:
        Color handle_hit(const HitRecord& hit) const;

        const int max_bounces_ = 8;
        mutable int bounces_remaining_ = max_bounces_;
        const Scene& scene_;
        Camera camera_;
        mutable std::mt19937 generator_;
    };
} // namespace pathtracer
