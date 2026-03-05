# Fractal Renderer with Raylib C++

A C++ application for real-time fractal generation and exploration using the Raylib library for low-level pixel buffer management.

This project implements two distinct types of fractals: escape-time algorithms (Mandelbrot) and recursive geometric structures (Fractal Tree). Both sets support dynamic scaling, where the fractal is re-computed and rendered on the CPU every frame during zoom operations to maintain mathematical precision.

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

---

## Gallery

<div align="center">
  <img src="assets/mandelbrot_base.png" width="800" alt="Mandelbrot Set Base View">
</div>

<br>

| Recursive Tree (Var A) | Recursive Tree (Var B) | Recursive Tree (Var C) | Recursive Tree (Var D) |
| :---: | :---: | :---: | :---: |
| <img src="assets/tree_var_a.png" width="180"> | <img src="assets/tree_var_b.png" width="180"> | <img src="assets/tree_var_c.png" width="180"> | <img src="assets/tree_var_d.png" width="180"> |

---

## About the Project
The core objective of this project is to implement mathematical rendering techniques directly on the CPU, using Raylib primarily as a backend to display the resulting pixel buffers and handle input events.

**Implemented Sets:**
* **Mandelbrot Set**: An escape-time fractal calculated using the iterative function $z_{n+1} = z_n^2 + c$. The renderer maps screen coordinates to the complex plane.
* **Recursive Fractal Trees**: Branching structures generated through recursion. Variations in branching angles and color interpolation are implemented to create different organic or geometric patterns.

**Technical Features:**
* **Dynamic Re-computation**: The fractal state is not scaled up or distorted like a static texture; it is fully re-calculated based on new viewport coordinates to ensure detail persistence at deep zoom levels.
* **Coordinate Mapping**: Translation of screen-space pixels to internal mathematical coordinate systems (complex plane for Mandelbrot, Cartesian for the Tree).
* **Modular Logic**: Separation between the main application loop (`main.cpp`) and the rendering/mathematical logic (`render.cpp`).

## Technologies

* **Language:** ISO C++17
* **Graphics API:** [Raylib](https://www.raylib.com/)
* **Compiler:** g++

## File Structure

* `main.cpp`: Event loop, input handling (zoom/pan), and window state management.
* `render.cpp`: Implementation of the Mandelbrot iteration loop, the Tree recursion, and pixel buffer manipulation.
* `render.hpp`: Function prototypes and shared rendering structures (e.g., color palettes).
* `.gitignore`: Project-specific exclusions.

## Build and Run

### Prerequisites

Ensure you have a C++ compiler and the Raylib development headers installed.

For Raylib installation instructions, refer to the official repository:
https://github.com/raysan5/raylib

### Compilation

Compile using g++ and link the necessary Raylib and system dependencies (standard on Linux):

```bash
g++ main.cpp render.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o fractals
