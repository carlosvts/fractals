// WIDTH/2, HEIGHT - 20, 300, 0, 20
// Renders a fractal based on which model
#include "raylib.h"
#include <cmath>
#include <random>
#include <experimental/random>
#include <vector> 

struct FractalTree
{
    std::vector<Color> colors;
    std::mt19937 mtgen;

    FractalTree();
    void render(float xpos, float ypos, float len, float ang, float thick, float zoom, bool colorful);
};