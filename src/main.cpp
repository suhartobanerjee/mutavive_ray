#include "../include/components.h"
#include "../dependencies/raylib-5.5_macos/include/raylib.h"
#include <memory>

// screen dimensions and FPS
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int TARGET_FPS = 120;

int main (int argc, char *argv[]) {
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(TARGET_FPS);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MutaVive: mutate to survive!");

    const int n_cells = 10;
    Cell** cells = new Cell*[n_cells];
    for (int i = 0; i < n_cells; i++) { 
        cells[i] = new Cell();
    }
    std::unique_ptr<Drug> drug = std::make_unique<Drug>();


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        for (int i = 0; i < n_cells; i++) {
            cells[i]->move();
            cells[i]->Draw();
        }
        drug->move();
        drug->Draw();

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    // cleaning up
    for (int i = 0; i < n_cells; i++) {
        delete cells[i];
        cells[i] = nullptr;
    }
    delete [] cells;
    cells = nullptr;

    return 0;
}
