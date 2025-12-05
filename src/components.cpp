#include "components.h"
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>

const float_t ENV_BOUNDARIES[4] = {-500., 500., -300., 300.};
const float_t TAU = 2 * M_PI;

Cell::Cell() {
    genome_size = 100;
    genome = new uint64_t[genome_size];

    static std::default_random_engine rng;
    std::uniform_real_distribution<float_t> colour_dist(0.0, 1.0);
    std::uniform_real_distribution<float_t> loc_distx(ENV_BOUNDARIES[0], ENV_BOUNDARIES[1]);
    std::uniform_real_distribution<float_t> loc_disty(ENV_BOUNDARIES[2], ENV_BOUNDARIES[3]);
    for (int i = 0; i <= 3; i++) {
        colour[i] = colour_dist(rng);
    }
    starting_coords[0] = loc_distx(rng);
    starting_coords[1] = loc_disty(rng);
    starting_coords[2] = 1.0;
    mutation_flag = 0;
    velocity = 50.;
    std::uniform_real_distribution<float_t> circumference(0.0, TAU);
    angle = circumference(rng);
    direction[0] = std::cos(angle);
    direction[1] = std::sin(angle);
    direction[2] = 0.0;
    random_walk = true;
    energy = 1.0;
    for (int i = 0; i <= 3; i++) {
        translate[i] = 0.0;
    }
}

Cell::~Cell() {
    std::cout << "deleting genome..." << std::endl;
    delete [] genome;
}

void Cell::move_cell() {
    static std::default_random_engine rng;
    if (this->random_walk) {
        std::bernoulli_distribution decision_dist(0.01);
        bool change_decision = decision_dist(rng);

        if (change_decision) {
            std::uniform_real_distribution<float_t> circumference(0.0, TAU);
            this->angle = circumference(rng);
            this->direction[0] = std::cos(this->angle);
            this->direction[1] = std::sin(this->angle);
            this->direction[2] = 0.0;
        }

        for (int i = 0; i <= 3; i++) {
            this->translate[i] += this->direction[i] * this->velocity;
        }

        // bounds check when we implement gui
    }
}

int main (int argc, char *argv[]) {
    Cell* cell = new Cell();

    std::ofstream ofs;
    std::ifstream ifs("translate.log");
    if (ifs) {
        remove("translate.log");
    }

    // printing the translate as a tuple to log file
    ofs.open("translate.log", std::ofstream::out | std::ofstream::app);
    if (!ofs) {
        return -1;
    }

    for (int i = 0; i <= 1e6; i++) {
        cell->move_cell();

        // appending to file
        ofs << "( ";
        for (auto& it : cell->translate) {
            ofs << it << ", ";
        }
        ofs << ")\n";
    }

    ofs.close();
    if (ofs.is_open()) {
        std::cout << "Could not close the file stream to log." << std::endl;
    }
    delete cell;

    return 0;
}
