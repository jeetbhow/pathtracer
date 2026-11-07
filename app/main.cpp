#include "pathtracer/renderer.hpp"
#include "pathtracer/object/material.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using std::cerr;
using std::make_shared;
using std::ofstream;

using namespace pathtracer;

int main()
{
    Image image{640, 360};
    Camera camera{image};

    Sphere glass_sphere{Vec3{-1.6f, -0.4f, -2.5f}, 0.6f};
    glass_sphere.material = make_shared<Dielectric>(Color{1.0f, 1.0f, 1.0f}, 1.5f);

    Sphere metal_sphere{Vec3{0, 0, -3}, 1.0f};
    metal_sphere.material = make_shared<Metal>(Color{0.9f, 0.9f, 0.9f}, 0.03f);

    const Scene scene
    {
        {
            make_shared<Sphere>(metal_sphere),
            make_shared<Sphere>(glass_sphere),
            make_shared<Sphere>(Vec3{1.7f, -0.2f, -3.5f}, 0.8f),
            make_shared<Sphere>(Vec3{0, -101, -3}, 100.0f)
        }
    };

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
