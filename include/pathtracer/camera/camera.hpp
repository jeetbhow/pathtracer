#pragma once

#include "pathtracer/geometry/ray.hpp"
#include "pathtracer/camera/image.hpp"
#include "pathtracer/camera/viewport.hpp"

#include <cstddef>
#include <random>

namespace pathtracer
{
    class Camera
    {
    public:
        Vec3 position = Vec3{0, 0, 0};
        Vec3 up = Vec3{0, 1, 0};
        Vec3 look_at = Vec3{0, 0, -1};
        float fov = 90;
        unsigned int samples = 8;

        Camera(Image& image);

        // Call after configuring the camera, before generating rays or rendering.
        void initialize();
        Image& image() const { return image_; }

        Ray generate_ray(std::size_t row, std::size_t col, std::mt19937& generator) const;

    private:
        static constexpr float FOCAL_LENGTH = 1.0f;
        Image& image_;                                  // Non-owning: image must outlive this Camera and any Renderer holding a copy.
        Viewport viewport_;
    };

} // namespace pathtracer
