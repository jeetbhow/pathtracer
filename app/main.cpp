#include "pathtracer/renderer.hpp"

#include <fstream>
#include <iostream>

using namespace pathtracer;

int main()
{
    const std::size_t IMG_WIDTH = 1280;
    const std::size_t IMG_HEIGHT = 720;

    const Camera camera{IMG_WIDTH, IMG_HEIGHT, 64};
    Sphere glass_sphere{Vec3{-1.6f, -0.4f, -2.5f}, 0.6f};
    glass_sphere.material = Material{MaterialType::Dielectric, Color{1.0f, 1.0f, 1.0f}, 0.0f, 1.5f};
    Sphere metal_sphere{Vec3{0, 0, -3}, 1.0f};
    metal_sphere.material = Material{MaterialType::Metal, Color{0.9f, 0.9f, 0.9f}, 0.03f};
    const Scene scene
    {
        {
            metal_sphere,
            glass_sphere,
            Sphere{Vec3{1.7f, -0.2f, -3.5f}, 0.8f},
            Sphere{Vec3{0, -101, -3}, 100.0f}
        }
    };

    const Renderer renderer{scene, camera};
    Image img = renderer.render();

    std::ofstream ofs{"image.ppm"};
    if (!ofs)
    {
        std::cerr << "Could not open image.ppm for writing\n";
        return 1;
    }

    img.write_ppm(ofs);
    ofs.close();
    if (!ofs)
    {
        std::cerr << "Could not write image.ppm\n";
        return 1;
    }
}
