#include "../include/camera.h"

static camera_t camera;

void init_camera(vec3_t *eye, vec3_t target, vec3_t up_helper) {
    camera.position = vec3_new(0, 0, 0);
    camera.direction = vec3_new(0, 0, 1);
    camera.rotation_angle = vec3_new(0, 0, 0);

    update_camera_vectors(eye, &target, &up_helper);
}

void update_camera_vectors(vec3_t *eye, vec3_t *target, vec3_t *up_helper) {
    camera.forward = vec3_sub(target, eye);
    vec3_normalize(&camera.forward);

    camera.right = vec3_cross(up_helper, &camera.forward);
    vec3_normalize(&camera.right);

    camera.up = vec3_cross(&camera.forward, &camera.right);
    vec3_normalize(&camera.up);
}

void update_camera_position(vec3_t position) {
    camera.position = position;
}

void update_camera_direction(vec3_t direction) {
    camera.direction = direction;
}

void update_camera_rotation(vec3_t rotation) {
    camera.rotation_angle = rotation;
}

void process_camera_movement(int direction, float delta_time) {
    if (direction == FORWARD) {
        camera.forward = vec3_mult(&camera.forward, 5.0 * delta_time);
        camera.position = vec3_add(&camera.position, &camera.forward);
    }

    if (direction == BACKWARD) {
        camera.forward = vec3_mult(&camera.forward, 5.0 * delta_time);
        camera.position = vec3_sub(&camera.position, &camera.forward);
    }

    if (direction == LEFT) {
        camera.right = vec3_mult(&camera.right, 5.0 * delta_time);
        camera.position = vec3_sub(&camera.position, &camera.right);
    }

    if (direction == RIGHT) {
        camera.right = vec3_mult(&camera.right, 5.0 * delta_time);
        camera.position = vec3_add(&camera.position, &camera.right);
    }

    if (direction == UP) {
        camera.up = vec3_mult(&camera.up, 5.0 * delta_time);
        camera.position = vec3_add(&camera.position, &camera.up);
    }

    if (direction == DOWN) {
        camera.up = vec3_mult(&camera.up, 5.0 * delta_time);
        camera.position = vec3_sub(&camera.position, &camera.up);
    }
}

void process_mouse_input(int *new_mouse_x, int *new_mouse_y, int *last_mouse_x,
    int *last_mouse_y, bool *first_mouse, float delta_time) {
   
    if (*first_mouse) {
        *last_mouse_x = *new_mouse_x;
        *last_mouse_y = *new_mouse_y;
        *first_mouse = false;
    }

    int x_offset = *new_mouse_x - *last_mouse_x;
    int y_offset = *new_mouse_y - *last_mouse_y;

    *last_mouse_x = *new_mouse_x;
    *last_mouse_y = *new_mouse_y;

    vec3_t *camera_rotation = get_camera_rotation();
    camera_rotation->y += -x_offset * 0.1 * delta_time;
    camera_rotation->x += -y_offset * 0.1 * delta_time;
    update_camera_rotation(*camera_rotation);
}

vec3_t* get_camera_position(void) {
    return &camera.position;
}

vec3_t* get_camera_direction(void) {
    return &camera.direction;
}

vec3_t* get_camera_rotation(void) {
    return &camera.rotation_angle;
}

vec3_t* get_camera_forward(void) {
    return &camera.forward;
}

vec3_t* get_camera_right(void) {
    return &camera.right;
}

vec3_t* get_camera_up(void) {
    return &camera.up;
}

void update_camera(void) {
    vec4_t target = {0, 0, 1, 0};
    vec3_t up = {0, 1, 0};
    mat4_t camera_yaw_rotation = mat4_make_rotation_y(get_camera_rotation());
    mat4_t camera_pitch_rotation = mat4_make_rotation_x(get_camera_rotation());
    mat4_t camera_rotation = mat4_mult_mat4(&camera_yaw_rotation, 
        &camera_pitch_rotation);
    update_camera_direction(vec3_from_vec4(mat4_mult_vec4(&camera_rotation, 
        &target)));
    vec3_t final_target = vec3_add(get_camera_position(), get_camera_direction());
    update_camera_vectors(get_camera_position(), &final_target, &up);
}

mat4_t mat4_look_at(vec3_t *eye, vec3_t *forward, vec3_t *right, vec3_t *up) {

    mat4_t view_matrix = {{
        {right->x, right->y, right->z, -vec3_dot(right, eye)},
        {up->x, up->y, up->z, -vec3_dot(up, eye)},
        {forward->x, forward->y, forward->z, -vec3_dot(forward, eye)},
        {0, 0, 0, 1}
    }};

    return view_matrix;
}
