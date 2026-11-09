#include "pathtracer/renderer.hpp"
#include "pathtracer/object/material.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

using std::cerr;
using std::make_unique;
using std::ofstream;

using namespace pathtracer;

int main()
{
    auto glass_sphere = make_unique<Sphere>(Vec3{-1.6f, -0.4f, -2.5f}, 0.6f);
    glass_sphere->material = make_unique<Dielectric>(Color{1.0f, 1.0f, 1.0f}, 1.5f);

    auto metal_sphere = make_unique<Sphere>(Vec3{0, 0, -3}, 1.0f);
    metal_sphere->material = make_unique<Metal>(Color{0.9f, 0.9f, 0.9f}, 0.03f);
    const Vec3 metal_sphere_pos = metal_sphere->center;

    Scene scene;
    scene.objects.push_back(std::move(metal_sphere));
    scene.objects.push_back(std::move(glass_sphere));
    scene.objects.push_back(make_unique<Sphere>(Vec3{1.7f, -0.2f, -3.5f}, 0.8f));
    scene.objects.push_back(make_unique<Sphere>(Vec3{0, -10001, -3}, 10000.0f));

    Image image{720, 400};
    Camera camera{image};

    camera.fov = 90;
    camera.look_at = metal_sphere_pos;
    camera.position = Vec3{0, 5, 4};
    camera.samples = 64;
    camera.initialize();

    const Renderer renderer{scene, camera};
    Image& img = renderer.render();

    ofstream ofs{"image.ppm"};
    if (!ofs)
    {
        cerr << "Could not open image.ppm for writing\n";
        return 1;
    }

    img.write_ppm(ofs);
    ofs.close();
    if (!ofs)
    {
        cerr << "Could not write image.ppm\n";
        return 1;
    }
}
