#include "pathtracer/renderer.hpp"
#include "pathtracer/geometry/sphere.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <optional>
#include <random>

using std::vector;

namespace pathtracer
{
    static std::optional<HitRecord> closest_hit(const Ray& ray, const Scene& scene)
    {
        std::optional<HitRecord> closest;
        for (const Sphere& sphere : scene.spheres)
        {
            const std::optional<HitRecord> hit = sphere.hit(ray);
            if (hit && (!closest || hit->t < closest->t))
            {
                closest.emplace(*hit);
            }
        }
        return closest;
    }

    static Color skybox_color(const Ray& ray)
    {
        const Vec3 unit_direction = ray.direction().normalized();
        const float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * Color{1.0f, 1.0f, 1.0f} + t * Color{0.5f, 0.7f, 1.0f};
    }

    static Color normal_color(const Vec3& normal)
    {
        return Color
        {
            (normal.x + 1.0f) / 2.0f,
            (normal.y + 1.0f) / 2.0f,
            (normal.z + 1.0f) / 2.0f
        };
    }

    Color Renderer::handle_hit(const HitRecord& hit) const
    {
        const Material& material = hit.object.material;
        Vec3 direction;

        switch (material.type)
        {
            case MaterialType::Diffuse:
                if (bounces_remaining_ <= 0) return Color{0, 0, 0};
                direction = (hit.normal + Sphere::random_vector(generator_)).normalized();
                break;
            case MaterialType::Metal:
                if (bounces_remaining_ < 0) return Color{0, 0, 0};
                direction = Vec3::reflect(hit.incoming.direction(), hit.normal).normalized();
                break;
            default:
                return Color{0, 0, 0};
        }

        const Ray scattered{hit.position, direction};
        const auto next = closest_hit(scattered, scene_);
        
        if (!next) 
        {
            return tint(skybox_color(scattered), material.albedo);
        }
        
        --bounces_remaining_;
        return tint(handle_hit(*next), material.albedo);
    }

    Image Renderer::render() const
    {
        Image image{camera_.width(), camera_.height()};
        for (std::size_t row = 0; row < camera_.height(); ++row)
        {
            for (std::size_t col = 0; col < camera_.width(); ++col)
            {
                const vector<Ray> rays = camera_.generate_rays(row, col, generator_);
                Color color{0, 0, 0};

                for (const Ray& ray : rays)
                {
                    const std::optional<HitRecord> hit = closest_hit(ray, scene_);

                    if (hit)
                    {
                        bounces_remaining_ = max_bounces_;
                        color += handle_hit(*hit);
                    }
                    else
                    {
                        color += skybox_color(ray);
                    }
                }

                image.set_pixel(row, col, color / camera_.samples_per_pixel());
            }
        }
        return image;
    }
} // namespace pathtracer
