#pragma once

#include "pathtracer/geometry/ray.hpp"
#include "pathtracer/geometry/plane.hpp"
#include "pathtracer/geometry/vec3.hpp"
#include "pathtracer/camera/image.hpp"

#include <cstddef>
#include <random>

namespace pathtracer {
    static constexpr float DEFAULT_FOCAL_LENGTH = 1.0f;
    static constexpr Vec3 DEFAULT_POSITION{0, 0, 0};
    static constexpr Vec3 DEFAULT_UP{0, 1, 0};
    static constexpr Vec3 DEFAULT_LOOK_AT{0, 0, -1};
    static constexpr float DEFAULT_FOV = 90;
    static constexpr unsigned int DEFAULT_SAMPLES = 8;
    static constexpr float DEFAULT_FOCUS_DISTANCE = 5.0f;
    static constexpr float DEFAULT_APERTURE_RADIUS = 1.0f;

    class Camera {
    public:
        Vec3 position = DEFAULT_POSITION;
        Vec3 look_at = DEFAULT_LOOK_AT;
        float focal_length = DEFAULT_FOCAL_LENGTH;
        float focus_distance = DEFAULT_FOCUS_DISTANCE;
        float fov = DEFAULT_FOV;
        unsigned int samples = DEFAULT_SAMPLES;

        Camera(Image& image);

        // Call once after configuring the camera, before generating rays or rendering.
        void initialize();
        Image& image() const {
            return image_;
        }

        Ray generate_ray(std::size_t row, std::size_t col, std::mt19937& generator) const;

    private:
        Image& image_; // Non-owning: image must outlive this Camera and any Renderer using it.

        // Pixel offsets are relative to the camera position.
        Vec3 pixel00_{};
        Vec3 pixel_du_{};
        Vec3 pixel_dv_{};

        Circle aperture_;
        Plane focus_plane_;

        void initialize_viewport(Vec3 forward);
    };

} // namespace pathtracer
