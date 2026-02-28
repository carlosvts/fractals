#include "render.hpp"

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "fractal");
    SetTargetFPS(24);
    std::string mode = "mendelbrot";

    // Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2) {WIDTH/2.0f, HEIGHT/2.0f};
    camera.offset = (Vector2) {WIDTH/2.0f, HEIGHT/2.0f};
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    
    Camera2D default_camera = camera; 
    
    FractalTree ft;
    MendelBrot mb;

    //if (mode == "mendelbrot") { mb.render(); }

    while (!WindowShouldClose())
    {
        // #################################
        //          Camera and Inputs
        // #################################
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
        // ##################################
        //            draw logic
        // #################################
        
        BeginDrawing();
            ClearBackground(BLACK);
            
            if (mode == "mendelbrot") 
            {
                mb.render();
                mb.handle_pan();
                mb.complex_camera();
                mb.update(); 
                DrawFPS(10, 10);
            }
    
            if (mode == "tree")
            {
                BeginMode2D(camera);
                    Vector2 top_left = GetScreenToWorld2D({0, 0}, camera);
                    Vector2 bottom_right = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);
                    ft.render(WIDTH/2.0f, HEIGHT - 20, 300, 0, 20, camera, top_left, bottom_right, false, MAX_DEPTH);   
                EndMode2D();
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
