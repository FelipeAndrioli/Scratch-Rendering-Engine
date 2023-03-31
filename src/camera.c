#include "../include/camera.h"

static camera_t camera;

void init_camera(vec3_t *eye, vec3_t target, vec3_t up_helper) {
    camera.position = vec3_new(0, 0, 0);
    camera.direction = vec3_new(0, 0, 1);
    camera.velocity = vec3_new(0, 0, 0);
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

void update_camera_velocity(vec3_t velocity) {
    camera.velocity = velocity;
}

void update_camera_rotation(vec3_t rotation) {
    camera.rotation_angle = rotation;
}

void process_camera_movement(int direction, float delta_time) {
    if (direction == FORWARD) {
        update_camera_velocity(vec3_mult(get_camera_direction(), 5.0 * delta_time));
        update_camera_position(vec3_add(get_camera_position(), get_camera_velocity()));
    }

    if (direction == BACKWARD) {
        update_camera_velocity(vec3_mult(get_camera_direction(), 5.0 * delta_time));
        update_camera_position(vec3_sub(get_camera_position(), get_camera_velocity()));
    }

    if (direction == LEFT) {
        return;
    }

    if (direction == RIGHT) {
        return;
    }
}

vec3_t* get_camera_position(void) {
    return &camera.position;
}

vec3_t* get_camera_direction(void) {
    return &camera.direction;
}

vec3_t* get_camera_velocity(void) {
    return &camera.velocity;
}

vec3_t* get_camera_rotation(void) {
    return &camera.rotation_angle;
}
