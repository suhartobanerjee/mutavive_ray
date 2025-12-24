#pragma once
#include <cstdint>
#include <cmath>
#include <memory>
#include <random>
#include "../dependencies/raylib-5.5_macos/include/raylib.h"

static std::default_random_engine rng;

// screen dimensions and FPS
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int BACKGROUND_SIZE = 256;
const int TARGET_FPS = 100;

const float_t TAU = 2 * M_PI;
const double_t ENERGY_COST = 1e-3;
const double_t P_DECISION = 1e-2;
const float_t ASTEROID_RADIUS = 28.;
const float_t SPACESHIP_RADIUS = 99.;
const float_t EARTH_RADIUS = 1280. * 0.2;
const double_t COLLISION_LAG_EARTH = 0.5;
const double_t COLLISION_LAG_SPACESHIP = 1.;
const double_t EARTH_RADIUS_OFFSET = 15.;


Vector2 get_direction(float_t);

enum Shapes {
    RECTANGLE,
    CIRCLE
};

struct Circle {
    Vector2* center;
    float_t radius;
};

struct Scoreboard {
    int32_t score;
};

struct EnvBounds {
    float_t x_min;
    float_t x_max;
    float_t y_min;
    float_t y_max;

    EnvBounds(float_t, Shapes);
};

struct Asteroid {
    float_t texture_width;
    float_t texture_height;
    float_t texture_rot;
    Rectangle texture_rect;
    Rectangle screen_rect;
    Vector2 origin;
    float_t velocity;
    float_t angle;
    Vector2 direction;
    double_t energy;
    EnvBounds bounds;
    Vector2 translate;
    std::uniform_real_distribution<float_t> circumference;
    std::bernoulli_distribution decision_dist;
    Color colour;
    bool bounced;
    double_t bounced_timer;

    // methods
    Asteroid();
    Vector2 bounce_direction(const Vector2&);
    Vector2 reverse_direction();
    void move();
    void Draw(const Texture2D&);
    void flip_bounced_state();
};

struct Spaceship {
    float_t texture_width;
    float_t texture_height;
    float_t texture_scale;
    Rectangle texture_rect;
    Rectangle screen_rect;
    Vector2 origin;
    float_t velocity;
    EnvBounds bounds;
    Vector2 translate;

    Spaceship();
    void move();
    void Draw(const Texture2D&);
    void check_collison_update(Asteroid&, Scoreboard&);
};

struct Earth {
    float_t texture_width;
    float_t texture_height;
    float_t texture_scale;
    float_t texture_rot;
    Rectangle texture_rect;
    Rectangle screen_rect;
    Vector2 origin;
    Vector2 center;

    Earth();
    void Draw(const Texture2D&);
    void check_collison_update(Asteroid&, Scoreboard&);
};

