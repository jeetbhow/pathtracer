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
        std::vector<std::shared_ptr<const Object>> objects;
    };

    class Renderer
    {
    public:
        Renderer(const Scene& scene, const Camera& camera, std::uint32_t seed = std::random_device{}())
            : scene_(scene), camera_(camera), generator_(seed)
        {
        }

        Image& render() const;
    private:
        Color handle_hit(const HitRecord& hit) const;

        const int max_bounces_ = 8;
        mutable int bounces_remaining_ = max_bounces_;
        Scene scene_;
        Camera camera_;
        mutable std::mt19937 generator_;
    };
} // namespace pathtracer
