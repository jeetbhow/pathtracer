#include "pathtracer/camera/camera.hpp"

namespace pathtracer
{
    Camera::Camera(Image& image, unsigned int samples)
        : image(image), samples(samples), viewport_(image.width, image.height)
    {
    }

    Ray Camera::generate_ray(std::size_t row, std::size_t col, std::mt19937& generator) const
    {
        std::uniform_real_distribution<float> distribution{0.0f, 1.0f};
        const float col_offset = distribution(generator);
        const float row_offset = distribution(generator);
        const Vec3 pixel_sample = viewport_.pixel00 + 
                                  viewport_.pixel_dv * (row + row_offset) + 
                                  viewport_.pixel_du * (col + col_offset);
        return Ray{origin, pixel_sample};
    }

} // namespace pathtracer
