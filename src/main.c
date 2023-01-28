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

vec3_t camera_position = {0, 0, 0};

// used by the perspective_projection
float fov_factor = 640;
// used by the orthographic_projection
//float fov_factor = 128;

bool is_running = false;
int previous_frame_time = 0;

void setup(void) {

    rendering_options.CULLING_BACKFACE = 1;
    rendering_options.RENDER_FILL_TRIANGLE = 1;
    rendering_options.RENDER_VERTEX = 0;
    rendering_options.RENDER_WIREFRAME = 0;

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

    // load specifically cube values
    load_cube_mesh_data();
    //load_model_mesh_data("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/cube/cube.obj");
    //load_model_mesh_data("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/f22/f22.obj");
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
            if (event.key.keysym.sym == SDLK_1)
                rendering_options.RENDER_VERTEX = !rendering_options.RENDER_VERTEX;
            if (event.key.keysym.sym == SDLK_2)
                rendering_options.RENDER_WIREFRAME = !rendering_options.RENDER_WIREFRAME;
            if (event.key.keysym.sym == SDLK_3)
                rendering_options.RENDER_FILL_TRIANGLE= !rendering_options.RENDER_FILL_TRIANGLE;
            if (event.key.keysym.sym == SDLK_4)
                rendering_options.CULLING_BACKFACE = !rendering_options.CULLING_BACKFACE;
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

    mesh.rotation.x += 0.01f;
    mesh.rotation.y += 0.01f;
    mesh.rotation.z += 0.01f;

    int n_faces = array_length(mesh.faces);
    for (int i = 0; i < n_faces; i++) {
        face_t mesh_face = mesh.faces[i];

        // build the set of three vectors from the triangle
        vec3_t face_vertices[3];
        // minus one because indices starts at 1
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec3_t transformed_vertices[3];

        // Transformations 
        for (int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];
            vec3_rotate(&transformed_vertex, &mesh.rotation);
            transformed_vertex.z += 5;

            transformed_vertices[j] = transformed_vertex;
        }

        // Projection
        vec2_t projected_points[3];

        for (int j = 0; j < 3; j++) {
            projected_points[j] = perspective_projection(transformed_vertices[j]);

            // scale and translate the projected points to the middle of the
            // screen
            projected_points[j].x += (window_width / 2);
            projected_points[j].y += (window_height / 2);
        }

        triangle_t projected_triangle = {
            {
                {projected_points[0].x, projected_points[0].y},
                {projected_points[1].x, projected_points[1].y},
                {projected_points[2].x, projected_points[2].y}
            },
            mesh_face.color
        };

        // save the projected triangle in an array of triangles to render
        // this is going to turn very slow in the future, but'll be fixed soon
        if (culling(transformed_vertices, camera_position) >= 0) {
            array_push(triangles_to_render, projected_triangle);
        }
    }
}

void render(void) {
    //draw_grid_lines(100, 100);
    draw_grid_dots(10, 10);

    int n_triangles = array_length(triangles_to_render);
    for (int i = 0; i < n_triangles; i++) {
        triangle_t triangle = triangles_to_render[i];
        draw(triangle, triangles_to_render[i].color);
    }

    array_free(triangles_to_render);
    
    render_color_buffer();
    clear_color_buffer(0xFF000000);
    SDL_RenderPresent(renderer);
}

void free_resources(void) {
    free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
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
    free_resources();

    return 0;
}
