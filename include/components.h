#pragma once
#include <cstdint>
#include <cmath>
#include <memory>
#include <random>
#include "../dependencies/raylib-5.5_macos/include/raylib.h"

static std::default_random_engine rng;

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
    Vector2 translate;
    std::unique_ptr<std::uniform_real_distribution<float_t>> circumference;
    std::unique_ptr<std::bernoulli_distribution> decision_dist;

    // methods
    Cell();
    void move_cell();
    void set_direction();
};

