// WIDTH/2, HEIGHT - 20, 300, 0, 20
// Renders a fractal based on which model
#include <cstdlib>
#include <iterator>
#include <new>
#include <raylib.h>
#include <cmath>
#include <cstdint>
#include <random>
#include <experimental/random>
#include <vector> 
#include <complex>

using namespace std::complex_literals;

constexpr int WIDTH = 1200;
constexpr int HEIGHT = 1200;
constexpr int MAX_DEPTH = 100;
constexpr int MAX_ITERATIONS = 20;

struct FractalTree
{
    std::vector<Color> colors;
    std::mt19937 mtgen;

    FractalTree();
    void render(float xpos, float ypos, float len, float ang, float thick, const Camera2D& camera, Vector2 top_left, Vector2 bottom_right, bool colorful, int depth);
};

struct MendelBrot
{
    // boundaries of real and imaginary plan
    double min_re = -2.0;
    double max_re =  1.0;
    double min_i  = -1.5; 
    double max_i  =  1.5;

    Image img{};            // cpu (RAM)
    Texture2D tex;          // texture (RAM)
    uint32_t* buff;    // pixel buffer
    
    MendelBrot();
    ~MendelBrot();

    void render();
    void update();
    int compute_escape(double a, double b);
    
    // return color per iterations
    uint32_t map_color(int iterations);
    
    template <typename T>
    inline T normalize_squared(const std::complex<T>&z);
};