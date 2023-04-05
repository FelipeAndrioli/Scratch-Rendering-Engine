#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../include/triangle.h"
#include "../include/matrix.h"
#include "../include/utils.h"
#include "../libs/dynamic_array/array.h"

#define FPS 60 
#define FRAME_TARGET_TIME (1000 / FPS)

typedef struct {
    bool RENDER_FILL_TRIANGLE;
    bool RENDER_WIREFRAME;
    bool RENDER_VERTEX;
    bool CULLING_BACKFACE;
    bool RENDER_TEXTURED;
} render_options;

typedef uint32_t color_t;

bool initialize_window(void);
void render_color_buffer();
void clear_color_buffer(color_t color);
void clear_z_buffer();
void destroy_window(void);
void draw_grid_dots(int column, int row);
void draw_grid_lines(int column, int row);
void draw_rect(int pos_x, int pos_y, int width, int height, color_t color);
void draw_wireframe(triangle_t triangle, color_t color);
void dda_draw_line(int x0, int y0, int x1, int y1, color_t color);
void draw_pixel(int x, int y, color_t color);
void draw_texel(int x, int y, triangle_t *face, upng_t *texture);
void draw_colored_pixel(int x, int y, triangle_t *face, color_t color);

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
void draw_filled_triangle(triangle_t triangle, color_t color);
void draw_triangle(triangle_t *face, color_t color, upng_t *texture);

float culling(vec3_t *face_normal, vec4_t *vertices, vec3_t camera_position);
void draw(triangle_t triangle);

void set_window_dim(int width, int height);
int get_window_width(void);
int get_window_height(void);

float get_zbuffer_at(int x, int y);
void update_zbuffer_at(int x, int y, float value);

void set_render_options(bool culling, bool fill_trianle, bool vertex, bool wireframe, bool textured);
void change_backface_culling(void);
void change_render_fill_triangle(void);
void change_render_vertex(void);
void change_render_wireframe(void);
void change_render_textured(void);

#endif
