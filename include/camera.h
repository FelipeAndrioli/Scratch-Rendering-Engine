#ifndef CAMERA_H
#define CAMERA_H

#include "./vector.h"

typedef struct {
    vec3_t position;
    vec3_t direction;
    vec3_t velocity;
    vec3_t rotation_angle;
} camera_t;

void init_camera(void);

void update_camera_position(vec3_t position);
void update_camera_direction(vec3_t direction);
void update_camera_velocity(vec3_t velocity);
void update_camera_rotation(vec3_t rotation);

vec3_t* get_camera_position(void);
vec3_t* get_camera_direction(void);
vec3_t* get_camera_velocity(void);
vec3_t* get_camera_rotation(void);

#endif
