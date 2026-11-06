#pragma once

#include <optional>
#include <string>

struct Vec3
{
    float x;
    float y;
    float z;

    static float dot(const Vec3& a, const Vec3& b);
    float length_squared() const;
    float length() const;
    Vec3 normalized() const;
};

constexpr Vec3 operator+(const Vec3& a, const Vec3& b) { return Vec3{a.x + b.x, a.y + b.y, a.z + b.z}; }
constexpr Vec3 operator-(const Vec3& a, const Vec3& b) { return Vec3{a.x - b.x, a.y - b.y, a.z - b.z}; }
constexpr Vec3 operator*(const Vec3& a, float scalar) { return Vec3{a.x * scalar, a.y * scalar, a.z * scalar}; }
constexpr Vec3 operator*(float scalar, const Vec3& a) { return a * scalar; }
constexpr Vec3 operator/(const Vec3& a, float scalar) { return a * (1.0f / scalar); }

class Ray {
public:
    Ray(const Vec3& origin, const Vec3& direction) : origin_(origin), direction_(direction) {}

    const Vec3 &direction() const { return direction_; }
    const Vec3 &origin() const { return origin_; }

    std::string to_string() const;

private:
    Vec3 origin_;
    Vec3 direction_;
};

class Sphere
{
public:
    Sphere(const Vec3& center, float radius) : radius_(radius), center_(center) {}

    std::optional<float> hit(const Ray& ray) const;
private:
    float radius_;
    Vec3 center_;
};
