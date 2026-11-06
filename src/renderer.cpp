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

    Color Renderer::handle_hit(const HitRecord& hit, int bounce) const
    {
        const Material& material = hit.object.material;
        switch (material.type)
        {
            case MaterialType::Diffuse:
                return tint(diffuse_color(hit, bounce), material.albedo);
            case MaterialType::Metal:
                return tint(reflect_color(hit, bounce), material.albedo);
        }
        return Color{0, 0, 0};
    }

    Color Renderer::diffuse_color(const HitRecord& hit, int bounce) const
    {
        if (bounce <= 0)
        {
            return Color{0, 0, 0};
        }

        Vec3 rnd = Sphere::random_vector(generator_);
        Vec3 sample_direction = (hit.normal + rnd).normalized();
        Ray sample{hit.position, sample_direction};

        const std::optional<HitRecord> next_hit = closest_hit(sample, scene_);
        if (next_hit)
        {
            return handle_hit(*next_hit, bounce - 1);
        }
        else
        {
            return skybox_color(sample);
        }

        return Color{0, 0, 0};
    }

    Color Renderer::reflect_color(const HitRecord& hit, int bounce) const
    {
        if (bounce < 0) 
        {
            return Color{0, 0, 0};
        }

        Vec3 refl_direction = Vec3::reflect(hit.incoming.direction(), hit.normal).normalized();
        Ray refl = Ray{hit.position, refl_direction};
    
        const std::optional<HitRecord> next_hit = closest_hit(refl, scene_);
        if (next_hit)
        {
            return handle_hit(*next_hit, bounce - 1);
        }
        else
        {
            return skybox_color(hit.incoming);
        }

        return Color{};
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
