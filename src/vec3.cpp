#include "../include/vec3.h"
#include <cmath>

// Vec3 methods
// zero constructor
Vec3::Vec3() : x(0.0), y(0.0), z(0.0) {}

Vec3::Vec3(float_t x_val, float_t y_val, float_t z_val)
    : x(x_val)
    , y(y_val)
    , z(z_val) {}


Vec3 Vec3::operator+(const Vec3& other) {
    return Vec3(
        this->x + other.x,
        this->y + other.y,
        this->z + other.z
    );
}

Vec3 Vec3::operator+(const float_t scalar) {
    return Vec3(
        this->x + scalar,
        this->y + scalar,
        this->z + scalar
    );
}

Vec3 Vec3::operator*(const Vec3& other) {
    return Vec3(
        this->x * other.x,
        this->y * other.y,
        this->z * other.z
    );
}

Vec3 Vec3::operator*(const float_t scalar) {
    return Vec3(
        this->x * scalar,
        this->y * scalar,
        this->z * scalar
    );
}
