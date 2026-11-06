#include "pathtracer/renderer.hpp"

namespace pathtracer {
    Color skybox_color(const Ray& ray)
    {
        const Vec3 unit_direction = ray.direction().normalized();
        const float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * Color{1.0f, 1.0f, 1.0f} + t * Color{0.5f, 0.7f, 1.0f};
    }

    Image render(const Camera& camera, const Sphere& sphere)
    {
        Image image{camera.width(), camera.height()};
        for (std::size_t row = 0; row < camera.height(); ++row)
        {
            for (std::size_t col = 0; col < camera.width(); ++col)
            {
                const Ray ray = camera.generate_ray(row, col);
                const Color color = sphere.hit(ray) ? Color{1.0f, 0.0f, 0.0f} : skybox_color(ray);
                image.set_pixel(row, col, color);
            }
        }
        return image;
    }
} // namespace pathtracer
