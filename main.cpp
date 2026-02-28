#include "raylib.h"
#include <cmath>
#include <random>
#include <vector> 

constexpr int WIDTH = 1200;
constexpr int HEIGHT = 1200;

std::mt19937 mtgen(std::random_device{}());
std::vector<Color> colors = {GOLD, ORANGE, YELLOW, BEIGE};

void DrawBranch(float xpos, float ypos, float lenght, float angle, float thickness)
{
    if (lenght <= 1) return;

    Vector2 start = {xpos, ypos};
    float x_end = xpos + sinf(angle) * lenght; 
    float y_end = ypos - cosf(angle) * lenght;
    Vector2 end = {x_end, y_end};
    std::uniform_int_distribution<std::size_t> randomcolor(0, colors.size() - 1);
    DrawLineEx(start, end, thickness, MAROON);
    
    float new_lenght = lenght * 0.70;
    float new_right_angle = angle + 15*DEG2RAD;
    float new_left_angle = angle - 15*DEG2RAD;
    float new_thickness = thickness * 0.60; 
    DrawBranch(x_end, y_end, new_lenght, new_right_angle, new_thickness);
    DrawBranch(x_end, y_end, new_lenght, new_left_angle, new_thickness);
}


int main(void)
{
    InitWindow(WIDTH, HEIGHT, "fractal");
    SetTargetFPS(24);
    
    // Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2) {WIDTH/2.0f, HEIGHT/2.0f};
    camera.offset = (Vector2) {WIDTH/2.0f, HEIGHT/2.0f};
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;

    Camera2D default_camera = camera; 

    while (!WindowShouldClose())
    {
        // camera targets mouse 
        float mousex = GetMouseX();
        float mousey = GetMouseY();
        
        if (GetMouseWheelMove())
        {
            Vector2 mousexy = GetScreenToWorld2D(GetMousePosition(), camera);
            // Camera zoom controls
            // Uses log scaling to provide consistent zoom speed
            camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove()*0.1f));
            
            Vector2 mousexy_after = GetScreenToWorld2D(GetMousePosition(), camera);

            camera.target.x += mousexy.x - mousexy_after.x;
            camera.target.y += mousexy.y - mousexy_after.y;
        }
        if (IsKeyPressed(KEY_R))
        {
            camera = default_camera;
        }
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(camera);
                DrawBranch(WIDTH/2, HEIGHT - 20, 300, 0, 20);             
            EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
