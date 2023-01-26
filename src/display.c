#include "../include/display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;

uint32_t *color_buffer = NULL;

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

void clear_color_buffer(uint32_t color) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            draw_pixel(x, y, color);
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

void draw_rect(int pos_x, int pos_y, int width, int height, uint32_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int current_x = pos_x + i;
            int current_y = pos_y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

void draw_triangle(triangle_t triangle, uint32_t color) {
    dda_draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x,
        triangle.points[1].y, color);
    dda_draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[2].x,
        triangle.points[2].y, color);
    dda_draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x,
        triangle.points[2].y, color);
}

void dda_draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
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

void draw_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < window_width && y >= 0 &&  y < window_height) {
        color_buffer[(window_width * y) + x] = color;
    }
}

void int_swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void float_swap(float *a, float *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
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

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {

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

void draw_filled_triangle(triangle_t triangle, uint32_t color) {
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

void draw(triangle_t triangle, uint32_t color) {
    if (rendering_options.RENDER_FILL_TRIANGLE) {
        draw_filled_triangle(triangle, color); 
    }
    if (rendering_options.RENDER_WIREFRAME) {
        draw_triangle(triangle, 0xFFFFFFFF);
    }
    if (rendering_options.RENDER_VERTEX) {
        draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFF0000);
        draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFF0000);
        draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFF0000);
    }
}


