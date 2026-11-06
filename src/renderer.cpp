#include "pathtracer/renderer.hpp"

#include <optional>

namespace pathtracer
{
    namespace
    {
        Color skybox_color(const Ray& ray)
        {
            const Vec3 unit_direction = ray.direction().normalized();
            const float t = 0.5f * (unit_direction.y + 1.0f);
            return (1.0f - t) * Color{1.0f, 1.0f, 1.0f} + t * Color{0.5f, 0.7f, 1.0f};
        }

        Color normal_color(const Vec3& normal)
        {
            return Color
            {
                (normal.x + 1.0f) / 2.0f,
                (normal.y + 1.0f) / 2.0f,
                (normal.z + 1.0f) / 2.0f
            };
        }
    }

    Image render(const Camera& camera, const Sphere& sphere)
    {
        Image image{camera.width(), camera.height()};
        for (std::size_t row = 0; row < camera.height(); ++row)
        {
            for (std::size_t col = 0; col < camera.width(); ++col)
            {
                const Ray ray = camera.generate_ray(row, col);
                const std::optional<HitRecord> hit = sphere.hit(ray);

                if (hit)
                {
                    image.set_pixel(row, col, normal_color(hit->normal));
                }
                else
                {
                    image.set_pixel(row, col, skybox_color(ray));
                }
            }
        }
        return image;
    }
} // namespace pathtracer
