#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

float vec2_length(vec2_t v);
float vec3_length(vec3_t v);

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

vec3_t vec3_rotate(vec3_t v, vec3_t rotation);
vec3_t vec3_translate(vec3_t v, vec3_t amount);
vec3_t vec3_scale(vec3_t v, vec3_t amount);
vec3_t vec3_scale_scalar(vec3_t v, float amount);

#endif
