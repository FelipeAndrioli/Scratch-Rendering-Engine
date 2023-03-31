#ifndef CAMERA_H
#define CAMERA_H

#include "./vector.h"

typedef struct {
    vec3_t forward;
    vec3_t right;
    vec3_t up;

    vec3_t position;
    vec3_t direction;
    vec3_t velocity;
    vec3_t rotation_angle;
} camera_t;

enum {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

void init_camera(vec3_t *eye, vec3_t target, vec3_t up_helper);

void update_camera_vectors(vec3_t *eye, vec3_t *target, vec3_t *up_helper);
void update_camera_position(vec3_t position);
void update_camera_direction(vec3_t direction);
void update_camera_velocity(vec3_t velocity);
void update_camera_rotation(vec3_t rotation);

void process_camera_movement(int direction, float delta_time);

vec3_t* get_camera_position(void);
vec3_t* get_camera_direction(void);
vec3_t* get_camera_velocity(void);
vec3_t* get_camera_rotation(void);

#endif
