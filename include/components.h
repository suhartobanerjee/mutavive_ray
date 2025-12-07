#pragma once
#include <cstdint>
#include <cmath>
#include <memory>
#include <random>
#include "../dependencies/raylib-5.5_macos/include/raylib.h"

static std::default_random_engine rng;

Vector2 get_direction(float_t);

enum Shapes {
    RECTANGLE,
    CIRCLE
};

struct EnvBounds {
    float_t x_min;
    float_t x_max;
    float_t y_min;
    float_t y_max;

    EnvBounds(float_t, Shapes);
};

struct Cell {
    uint64_t genome_size;
    std::unique_ptr<uint64_t> genome;
    //float_t colour[3];
    uint8_t mutation_flag;
    float_t velocity;
    float_t angle;
    Vector2 direction;
    bool random_walk;
    double_t energy;
    EnvBounds bounds;
    Vector2 translate;
    std::unique_ptr<std::uniform_real_distribution<float_t>> circumference;
    std::unique_ptr<std::bernoulli_distribution> decision_dist;
    float_t radius;

    // methods
    Cell();
    void move();
    void Draw();
};

struct Drug {
    float_t velocity;
    EnvBounds bounds;
    Vector2 translate;
    Vector2 size;
    Color colour;

    Drug();
    void move();
    void Draw();
};
