#include "../include/vector.h"

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
