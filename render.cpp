#include "render.hpp"
#include <complex>
#include <cstring>
#include <cstdint>
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

MendelBrot::MendelBrot()
    {
        buff = new uint32_t[WIDTH * HEIGHT];
        memset(buff, 0, WIDTH * HEIGHT * sizeof(uint32_t));
        // Image
        img.data = buff;
        img.height = HEIGHT;
        img.width = WIDTH;
        img.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

        // 2d texture
        tex = LoadTextureFromImage(img);
    }

MendelBrot::~MendelBrot()
    {
        UnloadTexture(tex);
        delete[] buff;
    }

void MendelBrot::render()
    {
        double real_width = max_re - min_re;
        double imaginary_width = max_i - min_i;
        double dx = real_width/WIDTH;
        double dy = imaginary_width/HEIGHT;

        
        for (int y{}; y < HEIGHT; y++)
        {
            for (int x{}; x < WIDTH; x++)
            {
                // c = a + bi
                // normalize pixel
                double normalized_x = static_cast<double>(x) / WIDTH;
                double normalized_y = static_cast<double>(y) / HEIGHT;


                double a = min_re + x * dx;
                double b = min_i + y * dy;

                int iterations = compute_escape(a, b);

                buff[static_cast<int>(y * WIDTH + x)] = map_color(iterations);
            }
        }
        printf("renderized\n");
    }

int MendelBrot::compute_escape(double a, double b)
{
    double zr = 0.0, zi = 0.0;
    double zr2 = 0.0, zi2 = 0.0;
    int i {};
    while (i < MAX_ITERATIONS && (zr2 + zi2) < 4)
    {
        zi = 2.0 * zr * zi + b; // b imaginary of c
        zr = zr2 - zi2 + a;    // a real of  c
        zr2 = zr * zr;
        zi2 = zi * zi;
        ++i;
    }

    if (i == MAX_ITERATIONS)
    {
        return 0L; // 0 long
    }
    else 
    {
        // a number has escaped 
        return i;
    }
}

void MendelBrot::update()
{
    UpdateTexture(tex, buff);
    DrawTexture(tex, 0, 0, WHITE);
}

template <typename T>
inline T MendelBrot::normalize_squared(const std::complex<T>& z)
{
    // we need to check if |z²| > threshold (in our case is 2)
    // so, |z| = sqrt(z_im², z_real²). So 2² = 4
    // squaring both sides
    return z.real() * z.real() + z.imag() * z.imag();
}

uint32_t MendelBrot::map_color(int iterations)
{
    if (iterations == 0 || iterations == MAX_ITERATIONS)
    {
        return 0x000000FF; // opacity at maximum
    }

    // normalized iteration value (used as perecntage)
    double t = static_cast<double>(iterations) / MAX_ITERATIONS;

    unsigned char r = static_cast<unsigned char>(t * 255); // red channel
    unsigned char g = 10; // a little of green
    // inverse of red
    unsigned char b = static_cast<unsigned char>((1.0f - t) * 255); // blue
    unsigned char a = 255; 

    // assuming rgba
    // R 00000000
    // G 00000000
    // B 00000000
    // A 00000000
    // 32bits in R, G, B, A 
    // shift red by 24 and so on so they will be in correct order (if rgba)
    return (r << 24) | (g << 16) | (b << 8) | a;
}