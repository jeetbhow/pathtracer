#pragma once

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3 operator+(const Vec3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vec3 operator-(const Vec3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vec3 operator*(float scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }
};

class Ray {
public:
    const Vec3& direction() const {
        return direction_;
    }

    const Vec3& origin() const {
        return origin_;
    }

private:
    Vec3 origin_;
    Vec3 direction_;
};