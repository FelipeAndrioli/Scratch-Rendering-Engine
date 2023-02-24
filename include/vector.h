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

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4_t;

vec2_t orthographic_projection(vec3_t point, float fov_factor);
vec2_t perspective_projection(vec3_t point, float fov_factor);

float vec2_length(vec2_t *v);
vec2_t vec2_add(vec2_t *a, vec2_t *b);
vec2_t vec2_sub(vec2_t *a, vec2_t *b);
vec2_t vec2_mult(vec2_t *v, float s);
vec2_t vec2_div(vec2_t *v, float s);
float vec2_dot(vec2_t *a, vec2_t *b);
void vec2_normalize(vec2_t *v);

float vec3_length(vec3_t *v);
vec3_t vec3_add(vec3_t *a, vec3_t *b);
vec3_t vec3_sub(vec3_t *a, vec3_t *b);
vec3_t vec3_mult(vec3_t *v, float s);
vec3_t vec3_div(vec3_t *v, float s);
vec3_t vec3_cross(vec3_t *a, vec3_t *b);
float vec3_dot(vec3_t *a, vec3_t *b);
void vec3_normalize(vec3_t *v);
vec3_t calculate_barycentric_weight(vec2_t *a, vec2_t *b, vec2_t *c, vec2_t *p);

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);
vec3_t vec3_rotate(vec3_t v, vec3_t rotation);

void vec3_translate(vec3_t *v, vec3_t *amount);
void vec3_scale(vec3_t *v, vec3_t *amount);
void vec3_scale_scalar(vec3_t *v, float amount);

vec4_t vec4_from_vec3(vec3_t v);
vec4_t vec4_from_vec2(vec2_t v);
vec3_t vec3_from_vec4(vec4_t v);

#endif
