#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../include/display.h"
#include "../include/vector.h"

#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {0, 0, -5};
vec3_t cube_rotation = {0.0f, 0.0f, 0.0f};
vec3_t cube_translation = {0.0f, 0.0f, 0.0f};
vec3_t cube_scalation = {1.0f, 1.0f, 1.0f};

float fov_factor = 640;

bool is_running = false;

void setup(void) {
    // Allocate the memory in bytes to holde the entire color buffer
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

    if (!color_buffer) {
        fprintf(stderr, "Error allocating memory for the color buffer\n");
        is_running = false;
    }

    // Create a SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    if (!color_buffer_texture) {
        fprintf(stderr, "Error creating the SDL Texture\n");
        is_running = false;
    }

    int point_count = 0;

    for (float x = -1; x <= 1; x+=0.25) {
        for (float y = -1; y <= 1; y+=0.25) {
            for (float z = -1; z <= 1; z+=0.25) {
                vec3_t new_point = {x, y, z};
                cube_points[point_count++] = new_point;
            }
        }
    }
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        // 'x' button of the window
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) 
                is_running = false;
            break;
    }
}

vec2_t orthographic_projection(vec3_t point) {
    vec2_t projected_point = {(fov_factor * point.x), (fov_factor * point.y)};

    return projected_point;
}

vec2_t perspective_projection(vec3_t point) {
    vec2_t projected_point = {(fov_factor * point.x ) / point.z, 
        (fov_factor * point.y) / point.z};

    return projected_point;
}

void update(void) {

    cube_rotation.x += 0.01f;
    cube_rotation.y += 0.01f;
    cube_rotation.z += 0.01f;

    for (int i = 0; i < N_POINTS; i++) {
        vec3_t point = cube_points[i];
        
        point = vec3_rotate_x(point, cube_rotation.x);
        point = vec3_rotate_y(point, cube_rotation.y);
        point = vec3_rotate_z(point, cube_rotation.z);
        point = vec3_translate(point, cube_translation);
        point = vec3_scale(point, cube_scalation);

        point.z -= camera_position.z;
        vec2_t projected_point = perspective_projection(point);
        //vec2_t projected_point = orthographic_projection(point);
        projected_points[i] = projected_point;
    }
}

void render(void) {
    //draw_grid_lines(100, 100);
    draw_grid_dots(10, 10);

    for (int i = 0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i];
        draw_rect(projected_point.x + window_width / 2, 
            projected_point.y + window_height / 2, 4, 4, 0xFFFF00FF);
    }
    
    render_color_buffer();
    clear_color_buffer(0xFF000000);
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    
    is_running = initialize_window();

    setup();

    while(is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}
