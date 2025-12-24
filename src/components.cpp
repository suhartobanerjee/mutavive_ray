#include <algorithm>
#include <cmath>
// #include <cstdint>
#include <random>
#include "components.h"
#include "../dependencies/raylib-5.5_macos/include/raylib.h"
#include "../dependencies/raylib-5.5_macos/include/raymath.h"


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


Asteroid::Asteroid() : bounds(ASTEROID_RADIUS, Shapes::CIRCLE) {
    texture_width = ASTEROID_RADIUS;
    texture_height = ASTEROID_RADIUS;
    texture_rot = 30.;
    texture_rect = Rectangle {0, 0, texture_width, texture_height};
    std::uniform_real_distribution<float_t> loc_distx(bounds.x_min, bounds.x_max);
    std::uniform_real_distribution<float_t> loc_disty(bounds.y_min, bounds.y_max);
    
    float_t x_loc = loc_distx(rng);
    while (x_loc > (SCREEN_WIDTH / 2.0f - 1280. * 0.2) &&
        x_loc < (SCREEN_WIDTH / 2.0f + 1280. * 0.2)) {
        x_loc = loc_distx(rng);
    }
    float_t y_loc = loc_disty(rng);
    while (y_loc > (SCREEN_WIDTH / 2.0f - 1280. * 0.2) &&
        y_loc < (SCREEN_WIDTH / 2.0f + 1280. * 0.2)) {
        y_loc = loc_disty(rng);
    }
    translate = Vector2 {x_loc, y_loc};

    screen_rect = Rectangle {
        translate.x,
        translate.y,
        texture_width,
        texture_height
    };
    origin = Vector2 { texture_width / 2.0f, texture_height / 2.0f };
    velocity = 100.;
    circumference = std::uniform_real_distribution<float_t>(0.0, TAU);
    angle = circumference(rng);
    direction = get_direction(angle);
    energy = 1.0;
    decision_dist = std::bernoulli_distribution(P_DECISION);
    colour = GRAY;
    bounced = false;
    bounced_timer = 0.0;
}

Vector2 Asteroid::bounce_direction(const Vector2 &platform) {
    Vector2 reflected = Vector2Reflect(this->translate, platform);
    Vector2 incident_norm = Vector2Normalize(platform);
    Vector2 reflected_norm = Vector2Normalize(reflected);
    this->angle = Vector2Angle(reflected_norm, incident_norm);
    // this->angle += PI;
    // if (this->angle > TAU) {
    //     this->angle -= TAU;
    // }
    return get_direction(this->angle);
}

Vector2 Asteroid::reverse_direction() {
    this->angle += PI;
    if (this->angle > TAU) {
        this->angle -= TAU;
    }
    return get_direction(this->angle);
}

void Asteroid::move() {
    bool change_decision = this->decision_dist(rng);

    if (change_decision) {
        this->angle = this->circumference(rng);
        this->direction = get_direction(this->angle);
    }

    // translating the asteroid
    Vector2 tmp_translate = this->translate + this->direction * this->velocity * GetFrameTime();

    // energy cost -> will add this when we develop further
    // this->energy -= ENERGY_COST;

    // Bounce off the walls
    bool out_of_bounds = tmp_translate.x <= this->bounds.x_min ||
        tmp_translate.x >= this->bounds.x_max ||
        tmp_translate.y <= this->bounds.y_min ||
        tmp_translate.y >= this->bounds.y_max;

    if (out_of_bounds) {
        this->direction = this->reverse_direction();
    }

    // finally, update the translation
    this->translate += this->direction * this->velocity * GetFrameTime();
}

void Asteroid::Draw(const Texture2D& texture) {
    this->screen_rect.x = this->translate.x;
    this->screen_rect.y = this->translate.y;
    DrawTexturePro(texture,
            this->texture_rect,
            this->screen_rect,
            this->origin,
            // this->texture_rot * GetTime(),
            0.,
            WHITE);
}

void Asteroid::flip_bounced_state() {
    this->bounced_timer -= GetFrameTime();
    if (this->bounced_timer <= 0. && this->bounced) {
        this->bounced = false;
    }
}


Spaceship::Spaceship() : bounds(SPACESHIP_RADIUS, Shapes::CIRCLE) {
    texture_width = 99.;
    texture_height = 75.;
    texture_scale = 0.7;
    texture_rect = Rectangle {0, 0, texture_width, texture_height};
    translate = Vector2 {(bounds.x_max - bounds.x_min) / 2.f, (bounds.y_max - bounds.y_min) / 2.f};
    screen_rect = Rectangle {
        translate.x,
        translate.y,
        texture_width * texture_scale,
        texture_height * texture_scale
    };
    origin = Vector2 { texture_width * texture_scale / 2.0f, texture_height * texture_scale / 2.0f };
    velocity = 350.;
}

void Spaceship::move() {
    // moving spaceship acc to input
    Vector2 movedir = {0., 0.};
    Vector2 next_pos = this->translate;

    if (IsKeyDown(KEY_RIGHT)) movedir.x++;
    if (IsKeyDown(KEY_LEFT)) movedir.x--;
    if (IsKeyDown(KEY_UP)) movedir.y--;
    if (IsKeyDown(KEY_DOWN)) movedir.y++;

    float_t length = Vector2Length(movedir);
    if (length > 0) {
        next_pos += Vector2Normalize(movedir) * this->velocity * GetFrameTime();
    }

    next_pos.x = std::clamp(next_pos.x, this->bounds.x_min, this->bounds.x_max);
    next_pos.y = std::clamp(next_pos.y, this->bounds.y_min, this->bounds.y_max);

    this->translate = next_pos;
}

void Spaceship::Draw(const Texture2D& texture) {
    this->screen_rect.x = this->translate.x;
    this->screen_rect.y = this->translate.y;
    DrawTexturePro(texture,
            this->texture_rect,
            this->screen_rect,
            this->origin,
            0.,
            WHITE);
}

void Spaceship::check_collison_update(Asteroid& asteroid, Scoreboard& scoreboard) {
    if (asteroid.bounced_timer > 0) {
        asteroid.bounced_timer -= GetFrameTime();
        return;
    }

    Rectangle asteroid_hitbox = Rectangle { 
        asteroid.translate.x - asteroid.origin.x,
        asteroid.translate.y - asteroid.origin.y,
        asteroid.screen_rect.width,
        asteroid.screen_rect.height
    };
    Rectangle spaceship_hitbox = Rectangle { 
        this->translate.x - this->origin.x,
        this->translate.y - this->origin.y,
        this->screen_rect.width,
        this->screen_rect.height
    };

    if (CheckCollisionRecs(spaceship_hitbox, asteroid_hitbox)) {
        asteroid.bounced_timer = COLLISION_LAG_SPACESHIP;
        asteroid.direction = asteroid.reverse_direction();
        asteroid.translate += asteroid.direction * asteroid.velocity * GetFrameTime();

        scoreboard.score++;
    }
}


Earth::Earth() {
    texture_width = 1280.;
    texture_height = 1280.;
    texture_scale = 0.2;
    texture_rot = 30.;
    texture_rect = Rectangle {0, 0, texture_width, texture_height};
    center = Vector2 { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    screen_rect = Rectangle {
        center.x,
        center.y,
        texture_width * texture_scale,
        texture_height * texture_scale
    };
    origin = Vector2 { texture_width * texture_scale / 2.0f, texture_height * texture_scale / 2.0f };
}

void Earth::Draw(const Texture2D& texture) {
    DrawTexturePro(texture,
            this->texture_rect,
            this->screen_rect,
            this->origin,
            this->texture_rot * GetTime(),
            WHITE);
}

void Earth::check_collison_update(Asteroid& asteroid, Scoreboard& scoreboard) {
    if (asteroid.bounced_timer > 0) {
        asteroid.bounced_timer -= GetFrameTime();
        return;
    }

    Rectangle asteroid_hitbox = Rectangle { 
        asteroid.translate.x - asteroid.origin.x,
        asteroid.translate.y - asteroid.origin.y,
        asteroid.screen_rect.width,
        asteroid.screen_rect.height
    };
    if(CheckCollisionCircleRec(this->center, this->origin.x - EARTH_RADIUS_OFFSET, asteroid_hitbox)) {
        asteroid.bounced_timer = COLLISION_LAG_EARTH;
        asteroid.direction = asteroid.reverse_direction();
        asteroid.translate += asteroid.direction * asteroid.velocity * GetFrameTime();
        scoreboard.score--;
    }    
}
