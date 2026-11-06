#pragma once

#include <cstddef>

#include "geometry.hpp"
#include "image.hpp"

class Camera 
{
public:
    explicit Camera(Image& image);

    Image& image() { return image_; }
    const Image& image() const { return image_; }

    Ray generate_ray(std::size_t row, std::size_t col) const;

private:
    Vec3 origin_ = Vec3{0, 0, 0};
    Image& image_;
    float vp_height_ = 2.0f;
    float focal_len_ = 1.0f;
};

Color ray_color(const Ray& r);
