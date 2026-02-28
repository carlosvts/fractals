#include "render.hpp"
#include <cmath>
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
        buff = new unsigned char[WIDTH * HEIGHT * 4];
        memset(buff, 0, WIDTH * HEIGHT * sizeof(unsigned char) * 4);
        // Image
        Image img = GenImageColor(WIDTH, HEIGHT, BLACK);

        // 2d texture
        tex = LoadTextureFromImage(img);

        UnloadImage(img);
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
        double zoom_level = real_width;
        
        for (int y{}; y < HEIGHT; y++)
        {
            for (int x{}; x < WIDTH; x++)
            {
                // z = a + bi

                double a = min_re + x * dx;
                double b = min_i + y * dy;

                double max_iter = 30 * 20 * log(1 / zoom_level + 1.0f);
                // z calculated here
                int iterations = compute_escape(a, b, max_iter);
                int idx = (y * WIDTH + x) * 4;

                map_color(iterations, &buff[idx], max_iter);
            }
        }
    }

int MendelBrot::compute_escape(double a, double b, double max_iterations)
{
    if (max_iterations == 0) max_iterations = MAX_ITERATIONS;

    double zr = 0.0, zi = 0.0;
    double zr2 = 0.0, zi2 = 0.0;
    
    // Optimizations
    double q = ((a - 0.25)*(a - 0.25)) + b * b;
    // cardioid test
    if (q * (q + (a - 0.25)) <= 0.25 * b * b) return 0L;
    // period-2 bulb test
    if ( ((a + 1) * (a + 1)) + b * b <= 1.0/16.0) return 0L;
    
    int i {};
    while (i < max_iterations && (zr2 + zi2) < 4)
    {
        zi = 2.0 * zr * zi + b; // b imaginary of c
        zr = zr2 - zi2 + a;    // a real of  c
        zr2 = zr * zr;
        zi2 = zi * zi;
        ++i;
    }

    if (i == max_iterations)
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

void MendelBrot::map_color(int iterations, unsigned char* buff, double max_iterations)
{   
    if (iterations == 0 || iterations == MAX_ITERATIONS)
    {
        buff[0] = 0;
        buff[1] = 0;
        buff[2] = 0;
        buff[3] = 255;
        return;
    }

    double t = (double)iterations / max_iterations;
    t = pow(t, 0.6);
    buff[0] = 255 * t;               // R
    buff[1] = 255 * t;                // G
    buff[2] = 255 * t;               // B
    buff[3] = 255;                         // A max opacity
}

void MendelBrot::complex_camera()
{
    // get mouse wheel movement
    float wheel = GetMouseWheelMove();

    // only zoom if there is input
    if (wheel == 0.0f) return;

    // smoother zoom factor
    double zoom = (wheel > 0) ? 0.9 : 1.1;

    // get mouse position in complex plane BEFORE zoom
    double dx = (max_re - min_re) / WIDTH;
    double dy = (max_i  - min_i)  / HEIGHT;

    double mouse_re = min_re + GetMouseX() * dx;
    double mouse_i  = min_i + GetMouseY() * dy;

    // compute new width and height after zoom
    double new_width  = (max_re - min_re) * zoom;
    double new_height = (max_i  - min_i)  * zoom;

    // adjust min and max so that zoom is centered at mouse position
    min_re = mouse_re - (GetMouseX() / (double)WIDTH) * new_width;
    max_re = min_re + new_width;

    min_i  = mouse_i - (GetMouseY() / (double)HEIGHT) * new_height;
    max_i  = min_i + new_height;
}

void MendelBrot::handle_pan()
{
    static Vector2 last_mouse = {0, 0};

    // if mouse button is pressed, update last position
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        last_mouse = GetMousePosition();
    }

    // if holding, move camera
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 current = GetMousePosition();

        // delta movement in screen space
        float dx_pixels = current.x - last_mouse.x;
        float dy_pixels = current.y - last_mouse.y;

        last_mouse = current;

        // convert to complex plane movement
        double dx = (max_re - min_re) / WIDTH;
        double dy = (max_i  - min_i)  / HEIGHT;

        double move_re = dx_pixels * dx;
        double move_i  = dy_pixels * dy;

        // move the view (note the minus to feel natural)
        min_re -= move_re;
        max_re -= move_re;
        min_i  -= move_i;
        max_i  -= move_i;
    }
}