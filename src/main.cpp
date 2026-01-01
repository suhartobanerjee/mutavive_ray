#include "components.h"
#include "../dependencies/raylib-5.5_macos/include/raylib.h"
#include "../dependencies/raylib-5.5_macos/include/raymath.h"
#include <cmath>
#include <cstdint>

const float_t PAUSE_TIMER = 1.;

int main (int argc, char *argv[]) {
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MutaVive: mutate to survive!");
    SetTargetFPS(TARGET_FPS);

    const int n_asteroids = 10;
    Asteroid asteroids[n_asteroids];
    for (int i = 0; i < n_asteroids; ++i) {
        asteroids[i] = Asteroid();
    }
    Spaceship spaceship = Spaceship();
    Earth earth = Earth();
    Scoreboard scoreboard {0};

    const Texture2D background_tex = LoadTexture("../assets/kenney_space-shooter-redux/Backgrounds/blue.png");
    const Texture2D asteroid_texture = LoadTexture("../assets/kenney_space-shooter-redux/PNG/Meteors/meteorGrey_small1.png");
    const Texture2D spaceship_texture = LoadTexture("../assets/kenney_space-shooter-redux/PNG/playerShip1_red.png");
    const Texture2D earth_texture = LoadTexture("../assets/kenney_planets/Planets/planet03.png");

    Rectangle screen_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    uint8_t level = 0;
    uint8_t highest_level = level;
    float_t pause_timer = PAUSE_TIMER;

    // Main game loop
    while (!WindowShouldClose())
    {
        // Checking for game over == score == 0
        if (scoreboard.score < 0 && level > 0) {
            float_t end_screen = 3.;
            while (end_screen > 0) {
                end_screen -= GetFrameTime();
                BeginDrawing();
                ClearBackground(GRAY);
                DrawText(TextFormat("Game over! Highest level reached is: %i", highest_level), 300., SCREEN_HEIGHT / 2., 36, BLACK);
                EndDrawing();
            }
            break;
        }
        // updating asteroid speed as part of levelling up
        if (floor(scoreboard.score / 10.0f) + 1 > level) {
            level++;
            highest_level = level;
            while (pause_timer > 0) {
                pause_timer -= GetFrameTime();
                BeginDrawing();
                ClearBackground(GRAY);
                DrawText(TextFormat("Level: %i", level), SCREEN_WIDTH / 2. - 50., SCREEN_HEIGHT / 2., 36, BLACK);
                EndDrawing();
            }
            pause_timer = PAUSE_TIMER;

            for (int i = 0; i < n_asteroids; ++i) {
                asteroids[i].velocity += ASTEROID_VELOCITY_UPDATE;
            }
        }
        // 1. game state update phase
        spaceship.move();
        for (int i = 0; i < n_asteroids; i++) {
            asteroids[i].move();

            spaceship.check_collison_update(asteroids[i], scoreboard);
            earth.check_collison_update(asteroids[i], scoreboard, level);
        }

        // 2. Rendering phase
        BeginDrawing();

            // clear and draw background
            ClearBackground(BLANK);
            DrawTexturePro(background_tex, screen_rect, screen_rect, Vector2Zero(), 0, WHITE);

            for (int i = 0; i < n_asteroids; ++i) {
                asteroids[i].Draw(asteroid_texture);
            }

            earth.Draw(earth_texture);
            spaceship.Draw(spaceship_texture);

            // showing FPS
            DrawText(TextFormat("CURRENT FPS: %i", GetFPS()), 10, 10, 20, GREEN);

            // showing score
            DrawText(TextFormat("Score: %i", scoreboard.score), 10, 30, 20, MAROON);

            // showing level
            DrawText(TextFormat("Level: %i", level), 10, 50, 20, BLACK);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
