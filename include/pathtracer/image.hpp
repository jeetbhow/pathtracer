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
        Image() = default;
        Image(std::size_t width, std::size_t height);

        std::size_t width() const
        {
            return width_;
        }
        std::size_t height() const
        {
            return height_;
        }

        void set_pixel(std::size_t row, std::size_t col, const Color& color);
        void write_ppm(std::ostream& output) const;

    private:
        std::size_t width_ = 0;
        std::size_t height_ = 0;
        std::vector<Color> pixels_;
    };

} // namespace pathtracer
