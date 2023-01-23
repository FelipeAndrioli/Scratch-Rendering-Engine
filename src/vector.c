#include "../include/vector.h"

float vec2_length(vec2_t v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
    vec2_t result = {a.x + b.x, a.y + b.y};
    return result;
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
    vec2_t result = {a.x - b.x, a.y - b.y};
    return result;
}

vec2_t vec2_mult(vec2_t v, float s) {
    vec2_t result = {v.x * s, v.y * s};
    return result;
}

vec2_t vec2_div(vec2_t v, float s) {
    vec2_t result = {v.x / s, v.y / s};
    return result;
}

float vec2_dot(vec2_t a, vec2_t b) {
    return a.x * b.x + a.y * b.y;
}

float vec3_length(vec3_t v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
    vec3_t result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
    vec3_t result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

vec3_t vec3_mult(vec3_t v, float s) {
    vec3_t result = {v.x * s, v.y * s, v.z * s};
    return result;
}

vec3_t vec3_div(vec3_t v, float s) {
    vec3_t result = {v.x / s, v.y / s, v.z / s};
    return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    vec3_t result = {
        a.y * b.y - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };

    return result;
}

float vec3_dot(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        v.x,
        v.y * cos(angle) - v.z * sin(angle),
        v.y * sin(angle) + v.z * cos(angle)
    };

    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        v.x * cos(angle) - v.z * sin(angle),
        v.y,
        v.x * sin(angle) + v.z * cos(angle)
    };

    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        v.x * cos(angle) - v.y * sin(angle),
        v.x * sin(angle) + v.y * cos(angle),
        v.z
    };

    return rotated_vector;
}

vec3_t vec3_rotate(vec3_t v, vec3_t rotation) {
    vec3_t rotated_vector = v;

    rotated_vector = vec3_rotate_x(rotated_vector, rotation.x);
    rotated_vector = vec3_rotate_y(rotated_vector, rotation.y);
    rotated_vector = vec3_rotate_z(rotated_vector, rotation.z);

    return rotated_vector;
}

vec3_t vec3_translate(vec3_t v, vec3_t amount) {
    vec3_t translated_vector = {
        v.x + amount.x,
        v.y + amount.y,
        v.z + amount.z
    };

    return translated_vector;
}

vec3_t vec3_scale(vec3_t v, vec3_t amount) {
    vec3_t translated_vector = {
        v.x * amount.x,
        v.y * amount.y,
        v.z * amount.z
    };

    return translated_vector;
}

vec3_t vec3_scale_scalar(vec3_t v, float amount) {
    vec3_t translated_vector = {
        v.x * amount,
        v.y * amount,
        v.z * amount
    };

    return translated_vector;
}
