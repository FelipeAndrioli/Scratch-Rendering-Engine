#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../libs/dynamic_array/array.h"
#include "../libs/uPNG/upng.h"

#include "../include/log.h"
#include "../include/display.h"
#include "../include/vector.h"
#include "../include/matrix.h"
#include "../include/mesh.h"
#include "../include/light.h"
#include "../include/triangle.h"
#include "../include/texture.h"
#include "../include/camera.h"

/*
    TODOs for the whole code

    TODO - After the course is finished, polish and refactor the whole code

    TODO - Completely remove dynamic_array 

    TODO - Remove this hardcoded MAX_TRIANGLES_PER_MESH calculating the amount
    of triangles we're going to render when loading obj files
    
    TODO - check if stb image can be used by C
    
    TODO - if above is true, then implement image load with stb image

    TODO - Apply lighting to textured models as well
*/

#define MAX_TRIANGLES_PER_MESH 10000
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render = 0;

vec3_t light_direction = {1.0, -1.0, -1.0};

bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0;

mat4_t proj_matrix;
mat4_t view_matrix;
mat4_t world_matrix;

void setup(void) {

    rendering_options.CULLING_BACKFACE = 1;
    rendering_options.RENDER_FILL_TRIANGLE = 1;
    rendering_options.RENDER_VERTEX = 0;
    rendering_options.RENDER_WIREFRAME = 0;
    rendering_options.RENDER_TEXTURED = 0;

    // Allocate the memory in bytes to holde the entire color buffer and z buffer
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    z_buffer = (float*) malloc(sizeof(float) * window_width * window_height);

    if (!color_buffer) {
        fprintf(stderr, "Error allocating memory for the color buffer\n");
        is_running = false;
    }

    // Create a SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    if (!color_buffer_texture) {
        fprintf(stderr, "Error creating the SDL Texture\n");
        is_running = false;
    }

    // need to convert the angle to radians
    float fov = M_PI / 3.0; 
    float aspect = (float)window_height / (float)window_width;
    float znear = 0.1;
    float zfar = 100.0;
    proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar); 

    //vec3_normalize(&light_direction);
    vec3_normalize(&global_light.direction);

    // load specifically cube values
    //load_cube_mesh_data();
    //load_model_mesh_data("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/cube/cube.obj");
    load_model_mesh_data("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/f22/f22.obj");
    load_png_texture_data("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/f22/f22.png");
   
    //load_png_texture_data("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/cube/cube.png");
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
            if (event.key.keysym.sym == SDLK_5)
                rendering_options.RENDER_TEXTURED = !rendering_options.RENDER_TEXTURED;
            if (event.key.keysym.sym == SDLK_w) {
                camera.velocity = vec3_mult(&camera.direction, 5.0 * delta_time);
                camera.position = vec3_add(&camera.position, &camera.velocity);
            }
            if (event.key.keysym.sym == SDLK_s) {
                camera.velocity = vec3_mult(&camera.direction, 5.0 * delta_time);
                camera.position = vec3_sub(&camera.position, &camera.velocity);
            }
            if (event.key.keysym.sym == SDLK_RIGHT)
                camera.rotation_angle.y += 1.0 * delta_time;
            if (event.key.keysym.sym == SDLK_LEFT)
                camera.rotation_angle.y -= 1.0 * delta_time;
            if (event.key.keysym.sym == SDLK_UP)
                camera.position.y += 3.0 * delta_time;
            if (event.key.keysym.sym == SDLK_DOWN)
                camera.position.y -= 3.0 * delta_time;

            break;
    }
}

void update(void) {

    // lock the execution until the execution reaches the desired FPS
    int wait_time = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (wait_time > 0 && wait_time <= FRAME_TARGET_TIME) {
        SDL_Delay(wait_time);
    }

    // Difference between the current and the previous frame, converted to seconds
    delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;

    // SDL_GetTicks() return how many miliseconds have passed since the 
    // application have started
    previous_frame_time = SDL_GetTicks();

    // Initialize the array
    //triangles_to_render = NULL;
    num_triangles_to_render = 0;

    //mesh.rotation.x += 0.6 * delta_time;
    //mesh.rotation.y += 0.01;
    //mesh.rotation.z += 0.01;

    //mesh.scale.x += 0.002;
    //mesh.scale.y += 0.001;

    //mesh.translation.x += 0.6 * delta_time;
    mesh.translation.z = 5.0;

    mat4_t scale_matrix = mat4_make_scale(&mesh.scale);
    mat4_t translation_matrix = mat4_make_translation(&mesh.translation);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(&mesh.rotation);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(&mesh.rotation);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(&mesh.rotation);

    // World matrix to scale, rotate and translate
    world_matrix = mat4_identity();
    world_matrix = mat4_mult_mat4(&scale_matrix, &world_matrix);
    world_matrix = mat4_mult_mat4(&rotation_matrix_x, &world_matrix);
    world_matrix = mat4_mult_mat4(&rotation_matrix_y, &world_matrix);
    world_matrix = mat4_mult_mat4(&rotation_matrix_z, &world_matrix);
    world_matrix = mat4_mult_mat4(&translation_matrix, &world_matrix);

    vec4_t target = {0, 0, 1, 0};
    vec3_t up = {0, 1, 0};
    mat4_t camera_yaw_rotation = mat4_make_rotation_y(&camera.rotation_angle);
    camera.direction = vec3_from_vec4(mat4_mult_vec4(&camera_yaw_rotation, 
        &target));
    vec3_t final_target = vec3_add(&camera.position, &camera.direction);

    view_matrix = mat4_look_at(&camera.position, &final_target, &up);

    int n_faces = array_length(mesh.faces);
    for (int i = 0; i < n_faces; i++) {
        face_t mesh_face = mesh.faces[i];

        // build the set of three vectors from the triangle
        vec3_t face_vertices[3];
        // minus one because indices starts at 1
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec4_t transformed_vertices[3];

        // Model Transformations 
        for (int j = 0; j < 3; j++) {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
            transformed_vertex = mat4_mult_vec4(&world_matrix, &transformed_vertex);
            // View Transformations
            transformed_vertex = mat4_mult_vec4(&view_matrix, &transformed_vertex);
            transformed_vertices[j] = transformed_vertex;
        }

        // Projection Transformations
        vec4_t projected_points[3];

        for (int j = 0; j < 3; j++) {
            //projected_points[j] = perspective_projection(vec3_from_vec4(transformed_vertices[j]));
            projected_points[j] = mat4_mult_vec4_project(&proj_matrix, &transformed_vertices[j]);

            // scale the projected points into the view port
            projected_points[j].x *= (window_width / 2.0);
            projected_points[j].y *= (window_height / 2.0);
            
            // Invert y values, our y on z buffer is pointing down but the model
            // y is pointing up, so the object is bein rendered upside down
            projected_points[j].y *= -1;

            // translate the projected points to the middle of the screen
            projected_points[j].x += (window_width / 2.0);
            projected_points[j].y += (window_height / 2.0);
        }

        vec3_t a = {transformed_vertices[0].x, transformed_vertices[0].y, transformed_vertices[0].z};
        vec3_t b = {transformed_vertices[1].x, transformed_vertices[1].y, transformed_vertices[1].z};
        vec3_t c = {transformed_vertices[2].x, transformed_vertices[2].y, transformed_vertices[2].z};
        vec3_t ab = vec3_sub(&a, &b);
        vec3_normalize(&ab);

        vec3_t ac = vec3_sub(&a, &c);
        vec3_normalize(&ac);

        vec3_t face_normal = vec3_cross(&ab, &ac);
        vec3_normalize(&face_normal);
    
        // Lighting
        float light_intensity = -vec3_dot(&global_light.direction, &face_normal);
        uint32_t triangle_color = light_apply_intensity(0xFFFF0000, light_intensity);
        // End lighting

        triangle_t projected_triangle = {
            {
                {projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w},
                {projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w},
                {projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w}
            },
            {
                {mesh_face.a_uv.u, mesh_face.a_uv.v},
                {mesh_face.b_uv.u, mesh_face.b_uv.v},
                {mesh_face.c_uv.u, mesh_face.c_uv.v}
            },
            triangle_color
        };


        // save the projected triangle in an array of triangles to render
        // this is going to turn very slow in the future, but'll be fixed soon
        vec3_t origin = {0, 0, 0};
        if (culling(&face_normal, transformed_vertices, origin) >= 0 
            && num_triangles_to_render < MAX_TRIANGLES_PER_MESH) {
            triangles_to_render[num_triangles_to_render++] = projected_triangle;
        }
    }
}

void render(void) {
    //draw_grid_lines(100, 100);
    draw_grid_dots(10, 10);

    for (int i = 0; i < num_triangles_to_render; i++) {
        triangle_t triangle = triangles_to_render[i];
        draw(triangle, triangles_to_render[i].color, mesh_texture);
    }

    //array_free(triangles_to_render);
    
    render_color_buffer();
    clear_color_buffer(0xFF000000);
    clear_z_buffer();
    SDL_RenderPresent(renderer);
}

void free_resources(void) {
    free(color_buffer);
    free(z_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
    array_free(mesh.normals);
    upng_free(png_texture);
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
