#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../libs/dynamic_array/array.h"

#include "../include/log.h"
#include "../include/display.h"
#include "../include/vector.h"
#include "../include/mesh.h"

triangle_t *triangles_to_render = NULL;

vec3_t camera_position = {0, 0, -5};
vec3_t cube_rotation = {0, 0, 0};

// used by the perspective_projection
float fov_factor = 640;
// used by the orthographic_projection
//float fov_factor = 128;

bool is_running = false;
int previous_frame_time = 0;

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

    // lock the execution until the execution reaches the desired FPS
    int wait_time = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (wait_time > 0 && wait_time < FRAME_TARGET_TIME) {
        SDL_Delay(wait_time);
    }

    // SDL_GetTicks() return how many miliseconds have passed since the 
    // application have started
    previous_frame_time = SDL_GetTicks();

    // Initialize the array
    triangles_to_render = NULL;

    cube_rotation.x += 0.01f;
    cube_rotation.y += 0.01f;
    cube_rotation.z += 0.01f;

    for (int i = 0; i < N_MESH_FACES; i++) {
        face_t mesh_face = mesh_faces[i];

        // build the set of three vectors from the triangle
        vec3_t face_vertices[3];
        // minus one because indices starts at 1
        face_vertices[0] = mesh_vertices[mesh_face.a - 1];
        face_vertices[1] = mesh_vertices[mesh_face.b - 1];
        face_vertices[2] = mesh_vertices[mesh_face.c - 1];

        triangle_t projected_triangle;

        // loop through all three vertices of the triangle stored in the face
        // vertices
        for (int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate(transformed_vertex, cube_rotation);
            transformed_vertex.z -= camera_position.z;
            vec2_t projected_point = perspective_projection(transformed_vertex);

            // scale and translate the projected points to the middle of the
            // screen
            projected_point.x += (window_width / 2);
            projected_point.y += (window_height / 2);
            projected_triangle.points[j] = projected_point;
        }
        // save the projected triangle in an array of triangles to render
        // this is going to turn very slow in the future, but'll be fixed soon
        array_push(triangles_to_render, projected_triangle);
    }
}

void render(void) {
    //draw_grid_lines(100, 100);
    draw_grid_dots(10, 10);

    int n_triangles = array_length(triangles_to_render);
    for (int i = 0; i < n_triangles; i++) {
        triangle_t triangle = triangles_to_render[i];
        draw_triangle(triangle, 0xFFFFFF00);
    }

    // Free the array of triangles to render every frame
    array_free(triangles_to_render);
    
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
