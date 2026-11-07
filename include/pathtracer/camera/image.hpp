#pragma once

#include <cstddef>
#include <iosfwd>
#include <vector>

#include "pathtracer/geometry/vec3.hpp"

namespace pathtracer
{
    class Image
    {
    public:
        // Dimensions are fixed at construction; only pixel values can change.
        const std::size_t width = 0;
        const std::size_t height = 0;

        Image() = default;
        Image(std::size_t width, std::size_t height);

        void set_pixel(std::size_t row, std::size_t col, const Color& color);
        void write_ppm(std::ostream& output) const;

    private:
        std::vector<Color> pixels_;
    };

} // namespace pathtracer
