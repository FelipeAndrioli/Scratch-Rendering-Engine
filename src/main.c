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
#include "../include/obj_parser.h"
#include "../include/mesh.h"
#include "../include/light.h"
#include "../include/triangle.h"
#include "../include/texture.h"
#include "../include/camera.h"
#include "../include/clipping.h"

/*
    TODOs for the whole code

    TODO - After the course is finished, polish and refactor the whole code

    TODO - Completely remove dynamic_array 

    TODO - Remove this hardcoded MAX_TRIANGLES_PER_MESH calculating the amount
    of triangles we're going to render when loading obj files
    
    TODO - check if stb image can be used by C
    
    TODO - if above is true, then implement image load with stb image

    TODO - Apply lighting to textured models as well

    TODO - Optimize process_input

    TODO - Create mouse struct

    TODO - Create keyboard struct

    TODO - Create keyboard states (pressed, released, ...)
*/

#define MAX_TRIANGLES_PER_MESH 10000
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render = 0;

bool is_running = false;
bool debug_mode = false;
int previous_frame_time = 0;
float delta_time = 0;

mat4_t proj_matrix;
mat4_t view_matrix;
mat4_t world_matrix;

int new_mouse_x = 0;
int new_mouse_y = 0;
int last_mouse_x = 0;
int last_mouse_y = 0;
bool first_mouse = true;

void setup(void) {
    // culling, fill_triangle, vertex, wireframe, textured
    set_render_options(1, 0, 0, 0, 1);

    init_light(vec3_new(0, 0, 1));
    init_camera(get_camera_position(), vec3_new(0, 0, 1), vec3_new(0, 1, 0));

    // need to convert the angle to radians
    float aspect_ratio_x = (float)get_window_width() / (float)get_window_height();
    float aspect_ratio_y = (float)get_window_height() / (float)get_window_width();
    float fov_y = M_PI / 3.0; 
    float fov_x = 2.0 * atan(tan(fov_y / 2) * aspect_ratio_x); 
    float znear = 0.1;
    float zfar = 100.0;
    proj_matrix = mat4_make_perspective(fov_y, aspect_ratio_y, znear, zfar); 

    init_frustum_planes(fov_x, fov_y, znear, zfar);

    vec3_normalize(get_light_direction_address());

    // load specifically cube values
    //load_png_texture_data("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/cube/cube.png");
    //load_model_mesh->data("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/cube/cube.obj");
    //load_model_mesh->data("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/f22/f22.obj");
    //load_png_texture_data("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/f22/f22.png");

    load_mesh("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/f117/f117.obj", 
        "C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/f117/f117.png", 
        vec3_new(1, 1, 1), vec3_new(9, 0, 0), vec3_new(0, 0, 0));
    
    load_mesh("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/efa/efa.obj", 
        "C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/efa/efa.png",
        vec3_new(1, 1, 1), vec3_new(3, 0, 0), vec3_new(0, 0, 0));

    load_mesh("C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/f22/f22.obj", 
        "C:/Users/Felipe/Documents/current_projects/Scratch-Rendering-Engine/assets/models/f22/f22.png", 
        vec3_new(1, 1, 1), vec3_new(-3, 0, 0), vec3_new(0, 0, 0));
}

void process_input(void) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            // 'x' button of the window
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                    break;
                } 
                if (event.key.keysym.sym == SDLK_1) {
                    change_render_vertex();
                    break;
                }
                if (event.key.keysym.sym == SDLK_2) {
                    change_render_wireframe();
                    break;
                }
                if (event.key.keysym.sym == SDLK_3) {
                    change_render_fill_triangle();
                    break;
                }
                if (event.key.keysym.sym == SDLK_4) {
                    change_backface_culling();
                    break;
                }
                if (event.key.keysym.sym == SDLK_5) {
                    change_render_textured();
                    break;
                }
                if (event.key.keysym.sym == SDLK_w) {
                    process_camera_movement(FORWARD, delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_s) {
                    process_camera_movement(BACKWARD, delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_a) {
                    process_camera_movement(LEFT, delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_d) {
                    process_camera_movement(RIGHT, delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_q) {
                    process_camera_movement(DOWN, delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_e) {
                    process_camera_movement(UP, delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    vec3_t *camera_rotation = get_camera_rotation();
                    camera_rotation->y += 1.0 * delta_time;
                    update_camera_rotation(*camera_rotation);
                    break;
                }
                if (event.key.keysym.sym == SDLK_LEFT) {
                    vec3_t *camera_rotation = get_camera_rotation();
                    camera_rotation->y -= 1.0 * delta_time;
                    update_camera_rotation(*camera_rotation);
                    break;
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    vec3_t *camera_position = get_camera_position();
                    camera_position->y += 3.0 * delta_time;
                    update_camera_position(*camera_position);
                    break;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    vec3_t *camera_position = get_camera_position();
                    camera_position->y -= 3.0 * delta_time;
                    update_camera_position(*camera_position);
                    break;
                }
                if (event.key.keysym.sym == SDLK_0) {
                    debug_mode = !debug_mode;
                }
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    printf("left click\n");
                    printf("x - %d\n", event.button.x);
                    printf("y - %d\n", event.button.y);
                    break;
                }

                if (event.button.button == SDL_BUTTON_RIGHT) {
                    printf("right click\n");
                    printf("x - %d\n", event.button.x);
                    printf("y - %d\n", event.button.y);
                    break;
                }
            case SDL_MOUSEMOTION:
                if (debug_mode) {
                    first_mouse = true;
                    break;
                };

                SDL_GetMouseState(&new_mouse_x, &new_mouse_y);
    
                process_mouse_input(&new_mouse_x, &new_mouse_y, &last_mouse_x,
                    &last_mouse_y, &first_mouse, delta_time);
            default:
                break;
        }
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

    //mesh->rotation.x += 0.6 * delta_time;
    //mesh->rotation.y += 0.01;
    //mesh->rotation.z += 0.01;

    //mesh->scale.x += 0.002;
    //mesh->scale.y += 0.001;

    //mesh->translation.x += 0.6 * delta_time;
    update_camera();
    view_matrix = mat4_look_at(get_camera_position(), get_camera_forward(),
        get_camera_right(), get_camera_up());

    for (int mesh_index = 0; mesh_index < get_num_meshes(); mesh_index++) {

        mesh_t *mesh = get_mesh(mesh_index);

        mesh->translation.z = 5.0;
        mesh->rotation.x += 0.6 * delta_time + (mesh_index * 0.001);
        mesh->rotation.y += 0.6 * delta_time + (mesh_index * 0.001);

        mat4_t scale_matrix = mat4_make_scale(&mesh->scale);
        mat4_t translation_matrix = mat4_make_translation(&mesh->translation);
        mat4_t rotation_matrix_x = mat4_make_rotation_x(&mesh->rotation);
        mat4_t rotation_matrix_y = mat4_make_rotation_y(&mesh->rotation);
        mat4_t rotation_matrix_z = mat4_make_rotation_z(&mesh->rotation);

        // World matrix to scale, rotate and translate
        world_matrix = mat4_identity();
        world_matrix = mat4_mult_mat4(&scale_matrix, &world_matrix);
        world_matrix = mat4_mult_mat4(&rotation_matrix_x, &world_matrix);
        world_matrix = mat4_mult_mat4(&rotation_matrix_y, &world_matrix);
        world_matrix = mat4_mult_mat4(&rotation_matrix_z, &world_matrix);
        world_matrix = mat4_mult_mat4(&translation_matrix, &world_matrix);

        int n_faces = array_length(mesh->faces);
        for (int i = 0; i < n_faces; i++) {
            face_t mesh_face = mesh->faces[i];

            // build the set of three vectors from the triangle
            vec3_t face_vertices[3];
            // minus one because indices starts at 1
            face_vertices[0] = mesh->vertices[mesh_face.a - 1];
            face_vertices[1] = mesh->vertices[mesh_face.b - 1];
            face_vertices[2] = mesh->vertices[mesh_face.c - 1];

            vec4_t transformed_vertices[3];

            // Model Transformations 
            for (int j = 0; j < 3; j++) {
                vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
                transformed_vertex = mat4_mult_vec4(&world_matrix, &transformed_vertex);
                // View Transformations
                transformed_vertex = mat4_mult_vec4(&view_matrix, &transformed_vertex);
                transformed_vertices[j] = transformed_vertex;
            }

            // Clipping
            polygon_t polygon = polygon_from_triangle(transformed_vertices, 
                mesh_face.a_uv, mesh_face.b_uv, mesh_face.c_uv);

            clip_polygon(&polygon);

            triangle_t clipped_triangles[MAX_CLIPPED_TRIANGLES];
            int n_clipped_triangles = 0;

            triangles_from_polygon(&polygon, clipped_triangles, &n_clipped_triangles);

            for (int t = 0; t < n_clipped_triangles; t++) {
                triangle_t clipped_triangle = clipped_triangles[t];
                // Projection Transformations
                vec4_t projected_points[3];

                for (int j = 0; j < 3; j++) {
                    //projected_points[j] = perspective_projection(vec3_from_vec4(transformed_vertices[j]));
                    //projected_points[j] = mat4_mult_vec4_project(&proj_matrix, &transformed_vertices[j]);
                    projected_points[j] = mat4_mult_vec4_project(&proj_matrix, &clipped_triangle.points[j]);

                    // scale the projected points into the view port
                    projected_points[j].x *= (get_window_width() / 2.0);
                    projected_points[j].y *= (get_window_height() / 2.0);
                    
                    // Invert y values, our y on z buffer is pointing down but the model
                    // y is pointing up, so the object is bein rendered upside down
                    projected_points[j].y *= -1;

                    // translate the projected points to the middle of the screen
                    projected_points[j].x += (get_window_width() / 2.0);
                    projected_points[j].y += (get_window_height() / 2.0);
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
                //float light_intensity = -vec3_dot(&global_light.direction, &face_normal);
                float light_intensity = -vec3_dot(get_light_direction_address(), &face_normal);
                uint32_t triangle_color = light_apply_intensity(0xFFFF0000, light_intensity);
                // End lighting

                triangle_t triangle_to_render = {
                    {
                        {projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w},
                        {projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w},
                        {projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w}
                    },
                    {
                        {clipped_triangle.texcoords[0].u, clipped_triangle.texcoords[0].v},
                        {clipped_triangle.texcoords[1].u, clipped_triangle.texcoords[1].v},
                        {clipped_triangle.texcoords[2].u, clipped_triangle.texcoords[2].v}
                    },
                    triangle_color,
                    mesh->texture
                };

                // save the projected triangle in an array of triangles to render
                // this is going to turn very slow in the future, but'll be fixed soon
                vec3_t origin = {0, 0, 0};
                if (culling(&face_normal, transformed_vertices, origin) >= 0 
                    && num_triangles_to_render < MAX_TRIANGLES_PER_MESH) {
                    triangles_to_render[num_triangles_to_render++] = triangle_to_render;
                }
            }
        }
    }
}

void render(void) {
    //draw_grid_lines(100, 100);
    clear_color_buffer(0xFF000000);
    clear_z_buffer();
    draw_grid_dots(10, 10);

    for (int i = 0; i < num_triangles_to_render; i++) {
        triangle_t triangle = triangles_to_render[i];
        draw(triangle);
    }

    render_color_buffer();
}

void free_resources(void) {
    free_meshes();
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
