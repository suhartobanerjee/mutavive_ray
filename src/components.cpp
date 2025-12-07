#include <cmath>
#include <cstdint>
#include <memory>
#include <random>
#include "../include/components.h"
#include "../dependencies/raylib-5.5_macos/include/raylib.h"
#include "../dependencies/raylib-5.5_macos/include/raymath.h"

const float_t TAU = 2 * M_PI;
const double_t ENERGY_COST = 1e-3;
const float_t TOL = 1e-3;
const double_t P_DECISION = 1e-2;
const float_t CELL_RADIUS = 30.;

EnvBounds::EnvBounds(float_t min_size, Shapes shape) {
    if (shape == Shapes::CIRCLE) {
        x_min = min_size;
        x_max = GetScreenWidth() - min_size;
        y_min = min_size;
        y_max = GetScreenHeight() - min_size;
    }
    if (shape == Shapes::RECTANGLE) {
        x_min = 0.;
        x_max = GetScreenWidth() - min_size;
        y_min = 0.;
        y_max = GetScreenHeight() - min_size;
    }
}

Vector2 get_direction(float_t angle) {
    return Vector2 {std::cos(angle), std::sin(angle)};
}

Cell::Cell() : bounds(CELL_RADIUS, Shapes::CIRCLE) {
    genome_size = 100;
    genome = std::make_unique<uint64_t>(genome_size);
    mutation_flag = 0;
    velocity = 200.;
    circumference = std::make_unique<std::uniform_real_distribution<float_t>>(0.0, TAU);
    angle = (*circumference)(rng);
    direction = get_direction(angle);
    random_walk = true;
    energy = 1.0;
    radius = CELL_RADIUS;
    std::uniform_real_distribution<float_t> loc_distx(bounds.x_min, bounds.x_max);
    std::uniform_real_distribution<float_t> loc_disty(bounds.y_min, bounds.y_max);
    translate = Vector2 {loc_distx(rng), loc_disty(rng)};
    decision_dist = std::make_unique<std::bernoulli_distribution>(P_DECISION);
}


void Cell::move() {
    if (this->random_walk) {
        bool change_decision = (*this->decision_dist)(rng);

        if (change_decision) {
            this->angle = (*this->circumference)(rng);
            this->direction = get_direction(this->angle);
        }

        // translating the cell
        Vector2 tmp_translate = this->translate + this->direction * this->velocity * GetFrameTime();

        // energy cost -> will add this when we develop further
        // this->energy -= ENERGY_COST;

        // bounds check when we implement gui
        bool out_of_bounds = tmp_translate.x <= this->bounds.x_min ||
            tmp_translate.x >= this->bounds.x_max ||
            tmp_translate.y <= this->bounds.y_min ||
            tmp_translate.y >= this->bounds.y_max;

        if (out_of_bounds) {
            this->angle += PI;
            if (this->angle > TAU) {
                this->angle -= TAU;
            }
            this->direction = get_direction(this->angle);
        }

        // finally, update the translation
        this->translate += this->direction * this->velocity * GetFrameTime();
    }
}

void Cell::Draw() {
    DrawCircleV(this->translate, this->radius, GRAY);
}


Drug::Drug() : bounds(70., Shapes::RECTANGLE) {
    velocity = 350.;
    translate = Vector2 {(bounds.x_max - bounds.x_min) / 2.f, (bounds.y_max - bounds.y_min) / 2.f};
    size = Vector2 {70., 70.};
    colour = Color {0, 139, 139, 255};
}

void Drug::move() {
    // moving drug acc to input
    Vector2 movedir = {0., 0.};
    if (IsKeyDown(KEY_RIGHT)) movedir.x++;
    if (IsKeyDown(KEY_LEFT)) movedir.x--;
    if (IsKeyDown(KEY_UP)) movedir.y--;
    if (IsKeyDown(KEY_DOWN)) movedir.y++;

    this->translate += Vector2Normalize(movedir) * this->velocity * GetFrameTime();

    if(this->translate.x >= this->bounds.x_max) this->translate.x = this->bounds.x_max;
    if(this->translate.x <= this->bounds.x_min) this->translate.x = this->bounds.x_min;
    if(this->translate.y <= this->bounds.y_min) this->translate.y = this->bounds.y_min;
    if(this->translate.y >= this->bounds.y_max) this->translate.y = this->bounds.y_max;
}

void Drug::Draw() {
    DrawRectangleV(this->translate, this->size, this->colour);
}
