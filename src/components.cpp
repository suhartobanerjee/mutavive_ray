#include <cmath>
#include <cstdint>
#include <memory>
#include <random>
#include "../include/components.h"
#include "../include/vec3.h"

const float_t ENV_BOUNDARIES[4] = {-500., 500., -300., 300.};
const float_t TAU = 2 * M_PI;
const double_t ENERGY_COST = 1e-3;
const float_t TOL = 1e-3;


Cell::Cell() {
    genome_size = 100;
    genome = std::make_unique<uint64_t>(genome_size);

    // std::uniform_real_distribution<float_t> colour_dist(0.0, 1.0);
    std::uniform_real_distribution<float_t> loc_distx(ENV_BOUNDARIES[0], ENV_BOUNDARIES[1]);
    std::uniform_real_distribution<float_t> loc_disty(ENV_BOUNDARIES[2], ENV_BOUNDARIES[3]);
    //for (int i = 0; i <= 3; i++) {
    //    colour[i] = colour_dist(rng);
    //}
    starting_coords = Vec3(loc_distx(rng), loc_disty(rng), 1.0 );
    mutation_flag = 0;
    velocity = 50.;
    circumference = std::make_unique<std::uniform_real_distribution<float_t>>(0.0, TAU);
    //circumference = std::make_unique<std::uniform_real_distribution<float_t>(0.0, TAU)>();
    angle = (*circumference)(rng);
    direction = Vec3(std::cos(angle), std::sin(angle), 0.0);
    random_walk = true;
    energy = 1.0;
    translate = Vec3();
    decision_dist = std::make_unique<std::bernoulli_distribution>(0.01);
}

void Cell::set_direction() {
    this->direction = Vec3(std::cos(this->angle), std::sin(this->angle), 0.0);
}


bool Cell::move_cell() {
    if (this->random_walk && this->energy >= 0.0) {
        bool change_decision = (*this->decision_dist)(rng);

        if (change_decision) {
            this->angle = (*this->circumference)(rng);
            this->set_direction();
        }

        // translating the cell
        this->translate = this->translate + this->direction * this->velocity;

        // energy cost
        this->energy -= ENERGY_COST;

        // bounds check when we implement gui


        return true;
    }
    return false;
}

