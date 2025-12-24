#include "components.h"
#include "../dependencies/raylib-5.5_macos/include/raylib.h"
#include "../dependencies/raylib-5.5_macos/include/raymath.h"

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

    // Main game loop
    while (!WindowShouldClose())
    {
        // 1. game state update phase
        spaceship.move();
        for (int i = 0; i < n_asteroids; i++) {
            asteroids[i].move();

            spaceship.check_collison_update(asteroids[i], scoreboard);
            earth.check_collison_update(asteroids[i], scoreboard);
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

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
