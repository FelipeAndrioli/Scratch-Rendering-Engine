#include "../include/camera.h"

static camera_t camera;

void init_camera(void) {

    camera.position = vec3_new(0, 0, 0);
    camera.direction = vec3_new(0, 0, 1);
    camera.velocity = vec3_new(0, 0, 0);
    camera.rotation_angle = vec3_new(0, 0, 0);
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
