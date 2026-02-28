#include "render.hpp"
#include <iostream>
#include <raylib.h>

FractalTree::FractalTree() 
    {
        // mersenne twister generator
        std::random_device rd;
        mtgen.seed(rd());

        colors = {GOLD, ORANGE, YELLOW, BEIGE};
    } 
    
void FractalTree::render(float x, float y, float length, float angle, float thickness, const Camera2D& camera, Vector2 top_left, Vector2 bottom_right, bool colorful, int depth)
    {
        // constraints
        if (--depth <= 0) return;
        if (length * camera.zoom <= 1.2f) return;
        if (length < 0.01f) return;

        float agressive_cut = length * 3.5f; 
        if (x < top_left.x - agressive_cut || x > bottom_right.x + agressive_cut ||
        y < top_left.y - agressive_cut || y > bottom_right.y + agressive_cut) return;
        
        Vector2 start = {x, y};
        float x_end = x + sinf(angle) * length; 
        float y_end = y - cosf(angle) * length;
        Vector2 end = {x_end, y_end};
        
        // check what is on the screen so we dont waste cpu
        float margin = length;
        bool is_visible = (x > top_left.x - margin && x < bottom_right.x + margin &&
            y > top_left.y - margin && y < bottom_right.y + margin);
            
        if (is_visible)
        {
            if (colorful) 
            {     
                std::uniform_int_distribution<std::size_t> randomcolor(0, colors.size() - 1);
                DrawLineEx(start, end, thickness, colors[randomcolor(mtgen)]); 
            }
            else { DrawLineEx(start, end, thickness, PURPLE); }
        }
        
        float new_length = length * 0.70;
        float new_right_angle = angle + 60*DEG2RAD;
        float new_left_angle = angle - 60*DEG2RAD;
        float new_thickness = thickness * 0.60; 
        render(x_end, y_end, new_length, new_right_angle, new_thickness, camera, top_left, bottom_right, colorful, depth);
        render(x_end, y_end, new_length, new_left_angle, new_thickness,  camera, top_left, bottom_right, colorful, depth);
    }

