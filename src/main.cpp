#include <memory>
#include "../include/components.h"
#include "../dependencies/raylib-5.5_macos/include/raylib.h"

// screen dimensions
const int screenWidth = 1280;
const int screenHeight = 720;

const int CELL_RADIUS = 30;

int main (int argc, char *argv[]) {

    std::unique_ptr<Cell> cell = std::make_unique<Cell>();

    InitWindow(screenWidth, screenHeight, "MutaVive: mutate to survive!");
    SetTargetFPS(60);


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(DARKGRAY);

        DrawCircle(cell->translate.x, cell->translate.y, CELL_RADIUS, GREEN);
        cell->move_cell();

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
