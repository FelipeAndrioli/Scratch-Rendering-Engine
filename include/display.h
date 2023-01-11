#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../include/triangle.h"

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* color_buffer_texture;

extern uint32_t *color_buffer;

extern int window_width;
extern int window_height;

bool initialize_window(void);
void render_color_buffer();
void clear_color_buffer(uint32_t color);
void destroy_window(void);
void draw_grid_dots(int column, int row);
void draw_grid_lines(int column, int row);
void draw_rect(int pos_x, int pos_y, int width, int height, uint32_t color);
void draw_triangle(triangle_t triangle, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_pixel(int x, int y, uint32_t color);

#endif
