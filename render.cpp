#include "render.hpp"

FractalTree::FractalTree() 
    {
        // mersenne twister generator
        std::random_device rd;
        mtgen.seed(rd());

        colors = {GOLD, ORANGE, YELLOW, BEIGE};
    } 
    
void FractalTree::render(float x, float y, float length, float angle, float thickness, bool colorful)
    {
        if (length <= 2) return;

        Vector2 start = {x, y};
        float x_end = x + sinf(angle) * length; 
        float y_end = y - cosf(angle) * length;
        Vector2 end = {x_end, y_end};
        std::uniform_int_distribution<std::size_t> randomcolor(0, colors.size() - 1);
        if (colorful) { DrawLineEx(start, end, thickness, colors[randomcolor(mtgen)]); }
        else { DrawLineEx(start, end, thickness, RAYWHITE); }
        
        
        float new_length = length * 0.70;
        float new_right_angle = angle + 15*DEG2RAD;
        float new_left_angle = angle - 15*DEG2RAD;
        float new_thickness = thickness * 0.60; 
        render(x_end, y_end, new_length, new_right_angle, new_thickness, colorful);
        render(x_end, y_end, new_length, new_left_angle, new_thickness, colorful);
    }
