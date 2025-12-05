#pragma once
#include <cstdint>
#include <cmath>
#include <random>

struct Vec3 {
    float_t x;
    float_t y;
    float_t z;

    Vec3();
    Vec3(float_t, float_t, float_t);
    Vec3 operator+(const Vec3&);
    Vec3 operator+(const float_t);
    Vec3 operator-(const Vec3&);
    Vec3 operator-(const float_t);
    Vec3 operator*(const Vec3&);
    Vec3 operator*(const float_t);
};

