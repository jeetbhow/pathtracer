#include "geometry.hpp"

Vec3 Vec3::operator+(const Vec3& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vec3 Vec3::operator*(float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}
