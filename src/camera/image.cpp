#include "pathtracer/camera/image.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <ostream>
#include <stdexcept>

using std::clamp;
using std::isfinite;
using std::length_error;
using std::logic_error;
using std::numeric_limits;
using std::ostream;
using std::out_of_range;
using std::size_t;
using std::sqrt;

namespace pathtracer
{

    static int channel_value(float value)
    {
        if (!isfinite(value))
        {
            return 0;
        }

        float clamped = clamp(value, 0.0f, 1.0f);
        float gamma_correction = sqrt(clamped);
        int rgb = static_cast<int>((gamma_correction * 255.0f) + 0.5f);
        return rgb;
    }

    Image::Image(size_t width, size_t height) : width(width), height(height)
    {
        if (height != 0 && width > numeric_limits<size_t>::max() / height)
        {
            throw length_error("Image dimensions are too large");
        }
        pixels_.resize(width * height, Color{0.0f, 0.0f, 0.0f});
    }

    void Image::set_pixel(size_t row, size_t col, const Color& color)
    {
        if (row >= height || col >= width)
        {
            throw out_of_range("Pixel coordinates are outside the image");
        }
        pixels_[row * width + col] = color;
    }

    void Image::write_ppm(ostream& output) const
    {
        if (width == 0 || height == 0)
        {
            throw logic_error("Cannot write an empty image");
        }

        output << "P3\n" << width << ' ' << height << "\n255\n";
        for (const Color& pixel : pixels_)
        {
            output << channel_value(pixel.x) << ' ' << channel_value(pixel.y) << ' '
                   << channel_value(pixel.z) << '\n';
        }
    }

} // namespace pathtracer
