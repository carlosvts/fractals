#include "raylib.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;

int main(void)
{
    InitWindow(800, 450, "fractal");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
             
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
