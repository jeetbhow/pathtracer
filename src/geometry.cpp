#include "geometry.hpp"

#include <cmath>
#include <sstream>

float Vec3::dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Vec3::length_squared() const
{
    return dot(*this, *this);
}

float Vec3::length() const
{
    return std::sqrt(length_squared());
}

Vec3 Vec3::normalized() const
{
    const float len = length();
    return len == 0.0f ? Vec3{0.0f, 0.0f, 0.0f} : *this / len;
}

std::string Ray::to_string() const
{
    std::ostringstream out;
    out << "Ray{origin: (" << origin_.x << ", " << origin_.y << ", " << origin_.z
        << "), direction: (" << direction_.x << ", " << direction_.y << ", " << direction_.z << ")}";
    return out.str();
}
