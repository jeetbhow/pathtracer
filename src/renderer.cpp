#include "pathtracer/renderer.hpp"
#include "pathtracer/object/material.hpp"
#include "pathtracer/object/object.hpp"
#include "pathtracer/geometry/vec3.hpp"

#include <optional>

using std::optional;
using std::size_t;

namespace pathtracer
{
    static optional<HitRecord> closest_hit(const Ray& ray, const Scene& scene)
    {
        optional<HitRecord> closest;
        for (const auto& object : scene.objects)
        {
            const optional<HitRecord> hit = object->hit(ray);
            if (hit && (!closest || hit->t < closest->t))
            {
                closest.emplace(*hit);
            }
        }
        return closest;
    }

    static Color skybox_color(const Ray& ray)
    {
        const Vec3 unit_direction = ray.direction.normalized();
        const float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * Color{1.0f, 1.0f, 1.0f} + t * Color{0.5f, 0.7f, 1.0f};
    }

    Color Renderer::handle_hit(const HitRecord& hit) const
    {
        if (bounces_remaining_ <= 0)
        {
            return Color{0, 0, 0};
        }

        const auto scatter = hit.material.scatter(hit.incoming, hit.normal, hit.entering, generator_);
        if (!scatter)
        {
            return Color{0, 0, 0};
        }

        const Ray scattered{hit.position, scatter->direction};
        optional<HitRecord> next = closest_hit(scattered, scene_);

        if (!next)
        {
            return tint(skybox_color(scattered), scatter->attenuation);
        }

        --bounces_remaining_;
        return tint(handle_hit(*next), scatter->attenuation);
    }

    Image& Renderer::render() const
    {
        Image& image = camera_.image;
        for (size_t row = 0; row < image.height; ++row)
        {
            for (size_t col = 0; col < image.width; ++col)
            {
                Color color{0, 0, 0};

                for (unsigned int sample = 0; sample < camera_.samples; ++sample)
                {
                    const Ray ray = camera_.generate_ray(row, col, generator_);
                    const optional<HitRecord> hit = closest_hit(ray, scene_);

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

                image.set_pixel(row, col, color / camera_.samples);
            }
        }
        return image;
    }
} // namespace pathtracer
