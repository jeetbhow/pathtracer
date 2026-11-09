#include "pathtracer/camera/camera.hpp"

#include <stdexcept>

using std::mt19937;
using std::size_t;
using std::uniform_real_distribution;

namespace pathtracer
{
    Camera::Camera(Image& image) : image_(image)
    {
    }

    void Camera::initialize()
    {
        const Vec3 direction = look_at - position;
        if (direction.length_squared() == 0.0f)
        {
            throw std::invalid_argument("Camera look_at must differ from position");
        }
        viewport_ = Viewport{image_, up, direction, fov, FOCAL_LENGTH};
    }

    Ray Camera::generate_ray(size_t row, size_t col, mt19937& generator) const
    {
        uniform_real_distribution<float> distribution{0.0f, 1.0f};
        const float col_offset = distribution(generator);
        const float row_offset = distribution(generator);
        const Vec3 pixel_sample = viewport_.pixel00 + 
                                  viewport_.pixel_dv * (row + row_offset) + 
                                  viewport_.pixel_du * (col + col_offset);
        return Ray{position, pixel_sample};
    }
} // namespace pathtracer
