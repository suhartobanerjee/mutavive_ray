#include <memory>
#include <fstream>
#include <iostream>
#include "../include/components.h"
#include "../include/vec3.h"
#include "../dependencies/raylib-5.5_macos/include/raylib.h"

int main (int argc, char *argv[]) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    std::unique_ptr<Cell> cell = std::make_unique<Cell>();

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

    bool res = false;
    for (int i = 0; i <= 1e6; i++) {
        res = cell->move_cell();

        if (res) {
            // appending to file
            ofs << "( ";
            ofs << cell->translate.x;
            ofs << ", ";
            ofs << cell->translate.y;
            ofs << ", ";
            ofs << cell->translate.z;
            ofs << ")";
            ofs << ", " << cell->energy << "\n";
        }
    }

    ofs.close();
    if (ofs.is_open()) {
        std::cout << "Could not close the file stream to log." << std::endl;
        return -1;
    }

    return 0;
}
