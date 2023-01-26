#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../include/triangle.h"

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

typedef struct {
    int RENDER_FILL_TRIANGLE;
    int RENDER_WIREFRAME;
    int RENDER_VERTEX;
    int CULLING_BACKFACE;
} render_options;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* color_buffer_texture;

extern uint32_t *color_buffer;

extern int window_width;
extern int window_height;

extern render_options rendering_options;

bool initialize_window(void);
void render_color_buffer();
void clear_color_buffer(uint32_t color);
void destroy_window(void);
void draw_grid_dots(int column, int row);
void draw_grid_lines(int column, int row);
void draw_rect(int pos_x, int pos_y, int width, int height, uint32_t color);
void draw_triangle(triangle_t triangle, uint32_t color);
void dda_draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_pixel(int x, int y, uint32_t color);

void swap(int *a, int *b);
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_filled_triangle(triangle_t triangle, uint32_t color);

void draw(triangle_t triangle, uint32_t color);
#endif
