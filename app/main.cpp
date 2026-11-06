#include "pathtracer/renderer.hpp"

#include <fstream>
#include <iostream>

using namespace pathtracer;

int main()
{
    const std::size_t IMG_WIDTH = 400;
    const std::size_t IMG_HEIGHT = 225;

    const Camera camera{IMG_WIDTH, IMG_HEIGHT};
    const Sphere sphere{Vec3{0, 0, -3}, 1.0f};
    const Image img = render(camera, sphere);

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
