#include "pathtracer/renderer.hpp"

#include <optional>

using namespace std;

namespace pathtracer
{
    Color skybox_color(const Ray& ray)
    {
        const Vec3 unit_direction = ray.direction().normalized();
        const float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * Color{1.0f, 1.0f, 1.0f} + t * Color{0.5f, 0.7f, 1.0f};
    }

    Color surface_normal(const Ray& r, float t, const Sphere& s)
    {
        Vec3 surf_pnt = r.at(t);
        Vec3 n = s.normal(surf_pnt);
        Color n_color{(n.x + 1) / 2, (n.y + 1) / 2, (n.z + 1) / 2};
        return n_color;
    }

    Image render(const Camera& cam, const Sphere& s)
    {
        Image image{cam.width(), cam.height()};
        for (std::size_t row = 0; row < cam.height(); ++row)
        {
            for (std::size_t col = 0; col < cam.width(); ++col)
            {
                const Ray r = cam.generate_ray(row, col);
                optional<float> t = s.hit(r);

                if (t)
                {
                    Color surf_norm = surface_normal(r, *t, s);
                    image.set_pixel(row, col, surf_norm);
                }
                else
                {
                    image.set_pixel(row, col, skybox_color(r));
                }
            }
        }
        return image;
    }
} // namespace pathtracer
