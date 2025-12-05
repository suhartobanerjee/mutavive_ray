#pragma once
#include <cstdint>
#include <cmath>

struct Cell {
    uint64_t genome_size;
    uint64_t* genome;
    float_t colour[3];
    int starting_coords[3];
    uint8_t mutation_flag;
    float_t velocity;
    float_t angle;
    float_t direction[3];
    bool random_walk;
    float_t energy;
    float_t translate[3];

    // methods
    Cell();
    ~Cell();
    void move_cell();
};
