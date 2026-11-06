#include "image.hpp"
#include "scene.hpp"
#include "geometry.hpp"

#include <fstream>
#include <iostream>

using namespace std;

int main()
{
    const size_t IMG_WIDTH = 400;
    const size_t IMG_HEIGHT = 225;

    Image img{IMG_WIDTH, IMG_HEIGHT};
    Camera camera{img};
    Sphere s{Vec3{0, 0, -3}, 1.0f};
    for (size_t row = 0; row < camera.image().height(); row++)
    {
        for (size_t col = 0; col < camera.image().width(); col++)
        {
            Ray r = camera.generate_ray(row, col);
            const Color color = s.hit(r) ? Color{1.0f, 0.0f, 0.0f} : skybox_color(r);
            img.set_pixel(row, col, color);
        }
    }

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
