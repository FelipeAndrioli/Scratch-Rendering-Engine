#include "../include/display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;

uint32_t *color_buffer = NULL;
float *z_buffer = NULL;

int window_width = 800;
int window_height = 600;

render_options rendering_options;

bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL\n");
        return false;
    }

    // Query and set fullscreen resolution
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    // Create a SDL Window
    window = SDL_CreateWindow("ScratchRenderingEngine.exe", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, 
        window_height, SDL_WINDOW_BORDERLESS);

    if (!window) {
        fprintf(stderr, "Error creating SDL window\n");
        return false;
    }

    // Create a SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer\n");
        return false;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void render_color_buffer() {
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, 
        (int)(window_width * sizeof(uint32_t)));
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(color_t color) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            draw_pixel(x, y, color);
        }
    }
}

void clear_z_buffer() {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            z_buffer[(window_width * y) + x] = 1.0;
        }
    }
}

void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_grid_dots(int column, int row) {
    for (int y = 0; y < window_height; y += row) {
        for (int x = 0; x < window_width; x += column) {
            draw_pixel(x, y, 0xFF333333);
        }
    }
}

void draw_grid_lines(int column, int row) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            if (x % column == 0 || y % row == 0) {
                draw_pixel(x, y, 0xFF333333);
            } else {
                draw_pixel(x, y, 0xFF000000);
            }
        }
    }
}

void draw_rect(int pos_x, int pos_y, int width, int height, color_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int current_x = pos_x + i;
            int current_y = pos_y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

void draw_wireframe(triangle_t triangle, color_t color) {
    dda_draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x,
        triangle.points[1].y, color);
    dda_draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[2].x,
        triangle.points[2].y, color);
    dda_draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x,
        triangle.points[2].y, color);
}

void dda_draw_line(int x0, int y0, int x1, int y1, color_t color) {
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)side_length;
    float y_inc = delta_y / (float)side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= side_length; i++) {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_pixel(int x, int y, color_t color) {
    if (x >= 0 && x < window_width && y >= 0 &&  y < window_height) {
        color_buffer[(window_width * y) + x] = color;
    }
}

void draw_texel(int x, int y, triangle_t *face, uint32_t *texture) {
    vec2_t p = {x, y};
    vec2_t a = {face->points[0].x, face->points[0].y};
    vec2_t b = {face->points[1].x, face->points[1].y};
    vec2_t c = {face->points[2].x, face->points[2].y};

    vec3_t weights = calculate_barycentric_weight(&a, &b, &c, &p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolated_u = (face->texcoords[0].u / face->points[0].w) * alpha 
        + (face->texcoords[1].u / face->points[1].w) * beta 
        + (face->texcoords[2].u / face->points[2].w) * gamma;
    float interpolated_v = (face->texcoords[0].v / face->points[0].w) * alpha 
        + (face->texcoords[1].v / face->points[1].w) * beta 
        + (face->texcoords[2].v / face->points[2].w) * gamma;
    float interpolated_w = (1 / face->points[0].w) * alpha 
        + (1 / face->points[1].w) * beta 
        + (1 / face->points[2].w) * gamma;

    interpolated_u /= interpolated_w;
    interpolated_v /= interpolated_w;

    // scale the uv coordinate (from 0 to 1) to the correct texture size (64 x 64 for example) 
    int texture_x = abs((int)(interpolated_u * texture_width)) % texture_width;
    int texture_y = abs((int)(interpolated_v * texture_height)) % texture_height;

    // with the current implementation, the closer the pixel, the bigger the
    // z value, so the check wouldn't work, therefore we need to adjustj it
    // to be the closer the pixel, the smaller the value
    interpolated_w = 1.0 - interpolated_w;

    // only draw the pixel and update the z buffer if the depth value is smaller
    // than the previous value on the pixel
    if (z_buffer[(window_width * y) + x] > interpolated_w) {
        draw_pixel(x, y, texture[(texture_width * texture_y) + texture_x]);
        // update z buffer
        z_buffer[(window_width * y) + x] = interpolated_w;
    }
}

void draw_colored_pixel(int x, int y, triangle_t *face, color_t color) {
    vec2_t p = {x, y};
    vec2_t a = {face->points[0].x, face->points[0].y};
    vec2_t b = {face->points[1].x, face->points[1].y};
    vec2_t c = {face->points[2].x, face->points[2].y};

    vec3_t weights = calculate_barycentric_weight(&a, &b, &c, &p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;
    
    float interpolated_w = (1 / face->points[0].w) * alpha 
        + (1 / face->points[1].w) * beta
        + (1 / face->points[2].w) * gamma;
    interpolated_w = 1.0 - interpolated_w;

    if (z_buffer[(window_width * y) + x] > interpolated_w) {
        draw_pixel(x, y, color);
        z_buffer[(window_width * y) + x] = interpolated_w;
    }
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color) {
    /*
        *Inverse Slope calculation
        
        Note: We need to get the slope in x, not in y as in the line equation
        y is always going to increase one by one, and is the x we need to calculate

        delta x = x1 - x0
        delta y = y1 - y0

        inverse_slope = delta x / delta y
    */

    float slope_left = (float)(x1 - x0) / (y1 - y0);
    float slope_right = (float)(x2 - x0) / (y2 - y0);
    float x_start = x0;
    float x_end = x0;

    for (int y = y0; y <= y2; y++) {
        dda_draw_line(x_start, y, x_end, y, color);
        x_start += slope_left;
        x_end += slope_right;
    }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color) {

    float slope_left = (float)(x2 - x0) / (y2 - y0);
    float slope_right = (float)(x2 - x1) / (y2 - y1);
    float x_start = x2;
    float x_end = x2;

    for (int y = y2; y >= y0; y--) {
        dda_draw_line(x_start, y, x_end, y, color);
        x_start -= slope_left;
        x_end -= slope_right;
    }
}

void draw_filled_triangle(triangle_t triangle, color_t color) {
    // Flat-top && flat-bottom triangle rendering technique
    if (triangle.points[0].y > triangle.points[1].y) {
        float_swap(&triangle.points[0].y, &triangle.points[1].y);
        float_swap(&triangle.points[0].x, &triangle.points[1].x);
    }
    if (triangle.points[1].y > triangle.points[2].y) {
        float_swap(&triangle.points[1].y, &triangle.points[2].y);
        float_swap(&triangle.points[1].x, &triangle.points[2].x);
    }
    if (triangle.points[0].y > triangle.points[1].y) {
        float_swap(&triangle.points[0].y, &triangle.points[1].y);
        float_swap(&triangle.points[0].x, &triangle.points[1].x);
    }

    if ((int)triangle.points[1].y == (int)triangle.points[2].y) {
        fill_flat_bottom_triangle(triangle.points[0].x, triangle.points[0].y,
            triangle.points[1].x, triangle.points[1].y, triangle.points[2].x,
            triangle.points[2].y, color);
    } else if ((int)triangle.points[0].y == (int)triangle.points[1].y) {
        fill_flat_top_triangle(triangle.points[0].x, triangle.points[0].y,
            triangle.points[1].x, triangle.points[1].y, triangle.points[2].x,
            triangle.points[2].y, color);
    } else {
        int mx = ((float)((triangle.points[2].x - triangle.points[0].x) * (
            triangle.points[1].y - triangle.points[0].y)) / (float)(
            triangle.points[2].y - triangle.points[0].y)) + triangle.points[0].x;
        int my = triangle.points[1].y;

        fill_flat_bottom_triangle(triangle.points[0].x, triangle.points[0].y, 
            triangle.points[1].x, triangle.points[1].y, mx, my, color);
        fill_flat_top_triangle(triangle.points[1].x, triangle.points[1].y, mx, my, 
            triangle.points[2].x, triangle.points[2].y, color);
    }
}

void draw_triangle(triangle_t *face, color_t color, uint32_t *texture) {
    // Flat-top && flat-bottom triangle rendering technique
    if (face->points[0].y > face->points[1].y) {
        float_swap(&face->points[0].y, &face->points[1].y);
        float_swap(&face->points[0].x, &face->points[1].x);
        float_swap(&face->points[0].z, &face->points[1].z);
        float_swap(&face->points[0].w, &face->points[1].w);
    
        float_swap(&face->texcoords[0].u, &face->texcoords[1].u);
        float_swap(&face->texcoords[0].v, &face->texcoords[1].v);
    }
    if (face->points[1].y > face->points[2].y) {
        float_swap(&face->points[1].y, &face->points[2].y);
        float_swap(&face->points[1].x, &face->points[2].x);
        float_swap(&face->points[1].z, &face->points[1].z);
        float_swap(&face->points[1].w, &face->points[2].w);
        
        float_swap(&face->texcoords[1].u, &face->texcoords[2].u);
        float_swap(&face->texcoords[1].v, &face->texcoords[2].v);
    }
    if (face->points[0].y > face->points[1].y) {
        float_swap(&face->points[0].y, &face->points[1].y);
        float_swap(&face->points[0].x, &face->points[1].x);
        float_swap(&face->points[0].z, &face->points[1].z);
        float_swap(&face->points[0].w, &face->points[1].w);
        
        float_swap(&face->texcoords[0].u, &face->texcoords[1].u);
        float_swap(&face->texcoords[0].v, &face->texcoords[1].v);
    }

    // invert the V's
    face->texcoords[0].v = 1.0 - face->texcoords[0].v;
    face->texcoords[1].v = 1.0 - face->texcoords[1].v;
    face->texcoords[2].v = 1.0 - face->texcoords[2].v;

    int x0 = face->points[0].x;
    int y0 = face->points[0].y;
    int x1 = face->points[1].x;
    int y1 = face->points[1].y;
    int x2 = face->points[2].x;
    int y2 = face->points[2].y;

    float inv_slope_left = 0;
    float inv_slope_right = 0;

    if (y1 - y0 != 0) inv_slope_left = (float)(x1 - x0) / abs(y1 - y0);
    if (y2 - y0 != 0) inv_slope_right = (float)(x2 - x0) / abs(y2 - y0);

    if (y1 - y0 != 0) {
        for (int y = y0; y <= y1; y++) {
            int x_start = x1 + (y - y1) * inv_slope_left;
            int x_end = x0 + (y - y0) * inv_slope_right;

            if (x_start > x_end) int_swap(&x_start, &x_end);

            for (int x = x_start; x <= x_end; x++) {
                if (rendering_options.RENDER_TEXTURED) {
                    draw_texel(x, y, face, texture);
                }
                if (rendering_options.RENDER_FILL_TRIANGLE) {
                    draw_colored_pixel(x, y, face, color);
                }
            }
        }
    }

    inv_slope_left = 0;
    inv_slope_right = 0;

    if (y2 - y1 != 0) inv_slope_left = (float)(x2 - x1) / abs(y2 - y1);
    if (y2 - y0 != 0) inv_slope_right = (float)(x2 - x0) / abs(y2 - y0);

    if (y2 - y1 != 0) {
        for (int y = y1; y <= y2; y++) {
            int x_start = x1 + (y - y1) * inv_slope_left;
            int x_end = x0 + (y - y0) * inv_slope_right;

            if (x_start > x_end) int_swap(&x_start, &x_end);

            for (int x = x_start; x <= x_end; x++) {
                if (rendering_options.RENDER_TEXTURED) {
                    draw_texel(x, y, face, texture);
                }
                if (rendering_options.RENDER_FILL_TRIANGLE) {
                    draw_colored_pixel(x, y, face, color);
                }
            }
        }
    }
}

float culling(vec3_t *face_normal, vec4_t *vertices, vec3_t camera_position) {
    if (!rendering_options.CULLING_BACKFACE) {
        return 0;
    }

    vec3_t vec_a = vec3_from_vec4(vertices[0]);

    vec3_t camera_ray = vec3_sub(&camera_position, &vec_a);
    float face_alignment = vec3_dot(face_normal, &camera_ray);

    return face_alignment;
}

void draw(triangle_t triangle, color_t color, uint32_t *texture) {
    if (rendering_options.RENDER_FILL_TRIANGLE || rendering_options.RENDER_TEXTURED) {
        draw_triangle(&triangle, color, texture); 
    }
    if (rendering_options.RENDER_VERTEX) {
        draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFF0000);
        draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFF0000);
        draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFF0000);
    }
    if (rendering_options.RENDER_WIREFRAME) {
        draw_wireframe(triangle, 0xFFFFFFFF);
    }
}
