#include "../include/triangle.h"

vec3_t calculate_triangle_normal(vec4_t vertices[3]) {

    vec3_t a = {vertices[0].x, vertices[0].y, vertices[0].z};
    vec3_t b = {vertices[1].x, vertices[1].y, vertices[1].z};
    vec3_t c = {vertices[2].x, vertices[2].y, vertices[2].z};
    vec3_t ab = vec3_sub(&a, &b);
    vec3_normalize(&ab);

    vec3_t ac = vec3_sub(&a, &c);
    vec3_normalize(&ac);

    vec3_t face_normal = vec3_cross(&ab, &ac);
    vec3_normalize(&face_normal);

    return face_normal;
}

vec3_t calculate_face_normal(vec4_t va, vec4_t vb, vec4_t vc) {

    vec3_t a = {va.x, va.y, va.z};
    vec3_t b = {vb.x, vb.y, vb.z};
    vec3_t c = {vc.x, vc.y, vc.z};
    
    vec3_t ab = vec3_sub(&a, &b);
    vec3_normalize(&ab);

    vec3_t ac = vec3_sub(&a, &c);
    vec3_normalize(&ac);

    vec3_t face_normal = vec3_cross(&ab, &ac);
    vec3_normalize(&face_normal);

    return face_normal;
}
