#include "../include/vector.h"

vec2_t orthographic_projection(vec3_t point, float fov_factor) {
    /*
        weak projection not considering anything else but the fov factor
    */
    vec2_t projected_point = {(fov_factor * point.x), (fov_factor * point.y)};

    return projected_point;
}

vec2_t perspective_projection(vec3_t point, float fov_factor) {
    /*
        weak projection not considering anything else but the fov factor and 
        the perspective divide
    */
    vec2_t projected_point = {(fov_factor * point.x ) / point.z, 
        (fov_factor * point.y) / point.z};

    return projected_point;
}

float vec2_length(vec2_t *v) {
    return sqrt(v->x * v->x + v->y * v->y);
}

vec2_t vec2_add(vec2_t *a, vec2_t *b) {
    vec2_t result = {a->x + b->x, a->y + b->y};
    return result;
}

vec2_t vec2_sub(vec2_t *a, vec2_t *b) {
    vec2_t result = {a->x - b->x, a->y - b->y};
    return result;
}

vec2_t vec2_mult(vec2_t *v, float s) {
    vec2_t result = {v->x * s, v->y * s};
    return result;
}

vec2_t vec2_div(vec2_t *v, float s) {
    vec2_t result = {v->x / s, v->y / s};
    return result;
}

float vec2_dot(vec2_t *a, vec2_t *b) {
    return a->x * b->x + a->y * b->y;
}

void vec2_normalize(vec2_t *v) {
    float mag = vec2_length(v);
    v->x /= mag;
    v->y /= mag;
}

float vec3_length(vec3_t *v) {
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

vec3_t vec3_add(vec3_t *a, vec3_t *b) {
    vec3_t result = {a->x + b->x, a->y + b->y, a->z + b->z};
    return result;
}

vec3_t vec3_sub(vec3_t *a, vec3_t *b) {
    vec3_t result = {a->x - b->x, a->y - b->y, a->z - b->z};
    return result;
}

vec3_t vec3_mult(vec3_t *v, float s) {
    vec3_t result = {v->x * s, v->y * s, v->z * s};
    return result;
}

vec3_t vec3_div(vec3_t *v, float s) {
    vec3_t result = {v->x / s, v->y / s, v->z / s};
    return result;
}

vec3_t vec3_cross(vec3_t *a, vec3_t *b) {
    vec3_t result = {
        a->y * b->z - a->z * b->y,
        a->z * b->x - a->x * b->z,
        a->x * b->y - a->y * b->x
    };

    return result;
}

float vec3_dot(vec3_t *a, vec3_t *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vec3_normalize(vec3_t *v) {
    float mag = vec3_length(v);
    v->x /= mag;
    v->y /= mag;
    v->z /= mag;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
    vec3_t r = {
        v.x,
        v.y * cos(angle) - v.z * sin(angle),
        v.y * sin(angle) + v.z * cos(angle)
    };
    return r;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
    vec3_t r = {
        v.x * cos(angle) - v.z * sin(angle),
        v.y,
        v.x * sin(angle) + v.z * cos(angle)
    };
    return r;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
    vec3_t r = {
        v.x * cos(angle) - v.y * sin(angle),
        v.x * sin(angle) + v.y * cos(angle),
        v.z
    };
    return r;
}

vec3_t vec3_rotate(vec3_t v, vec3_t rotation) {
    /*
        we're not going to do the rotation via reference because when we 
        calculate the rotated z value we're using the y value, not the original
        but the y rotated one, so this is messing with the form of the object
    */
    vec3_t r = v;
    r = vec3_rotate_x(r, rotation.x);
    r = vec3_rotate_y(r, rotation.y);
    r = vec3_rotate_z(r, rotation.z);

    return r;
}

void vec3_translate(vec3_t *v, vec3_t *amount) {
    v->x += amount->x;
    v->y += amount->y;
    v->z += amount->z;
}

void vec3_scale(vec3_t *v, vec3_t *amount) {
    v->x *= amount->x;
    v->y *= amount->y;
    v->z *= amount->z;
}

void vec3_scale_scalar(vec3_t *v, float amount) {
    v->x *= amount;
    v->y *= amount;
    v->z *= amount;
}

vec4_t vec4_from_vec3(vec3_t v) {
    vec4_t r = {v.x, v.y, v.z, 1.0};

    return r;
}

vec3_t vec3_from_vec4(vec4_t v) {
    vec3_t r = {v.x, v.y, v.z};

    return r;
}
