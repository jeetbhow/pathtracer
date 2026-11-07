#include "pathtracer/renderer.hpp"
#include "pathtracer/geometry/hit_record.hpp"
#include "pathtracer/geometry/sphere.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <optional>
#include <random>

using std::optional;
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

    Color Renderer::handle_hit(const HitRecord& hit) const
    {
        if (bounces_remaining_ <= 0)
        {
            return Color{0, 0, 0};
        }

        const Material& material = hit.object.material;
        const Vec3 incoming = hit.incoming.direction().normalized();
        const Vec3& normal = hit.normal;
        Vec3 direction;

        switch (material.type)
        {
            case MaterialType::Diffuse:
                direction = (normal + Sphere::random_vector(generator_));
                break;
            case MaterialType::Metal:
                direction = Vec3::reflect(incoming, normal);

                if (material.roughness > 0.0f)
                {
                    direction = direction + material.roughness * Sphere::random_vector(generator_);
                }

                if (Vec3::dot(direction, normal) <= 0.0f)
                {
                    return Color{0, 0, 0};
                }

                direction = direction.normalized();
                break;
            case MaterialType::Dielectric:
            {
                const float incident_ior = hit.entering ? 1.0f : material.ior;
                const float transmitted_ior = hit.entering ? material.ior : 1.0f;
                std::uniform_real_distribution<float> sample{0.0f, 1.0f};
                direction = Vec3::refract(incoming, normal, incident_ior, transmitted_ior, sample(generator_));
                break;
            }
            default:
                return Color{0, 0, 0};
        }

        const Ray scattered{hit.position, direction.normalized()};
        optional<HitRecord> next = closest_hit(scattered, scene_);

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
