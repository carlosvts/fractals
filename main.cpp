#include "render.hpp"

constexpr int WIDTH = 1200;
constexpr int HEIGHT = 1200;

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
    
    FractalTree ft = FractalTree();
    
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
                ft.render(WIDTH/2.0f, HEIGHT - 20, 300, 0, 20, camera.zoom, false);  
            EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
