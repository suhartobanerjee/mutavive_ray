#pragma once
#include <cstdint>
#include <cmath>
#include <memory>
#include <random>
#include "vec3.h"

static std::default_random_engine rng;

struct Cell {
    uint64_t genome_size;
    std::unique_ptr<uint64_t> genome;
    //float_t colour[3];
    Vec3 starting_coords;
    uint8_t mutation_flag;
    float_t velocity;
    float_t angle;
    Vec3 direction;
    bool random_walk;
    double_t energy;
    Vec3 translate;
    std::unique_ptr<std::uniform_real_distribution<float_t>> circumference;
    std::unique_ptr<std::bernoulli_distribution> decision_dist;

    // methods
    Cell();
    bool move_cell();
    void set_direction();
};

