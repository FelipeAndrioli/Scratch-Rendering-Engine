#include "../include/matrix.h"

mat4_t mat4_identity(void) {
    mat4_t m = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }};
    
    return m;
}

mat4_t mat4_make_scale(vec3_t *scale) {
    mat4_t m = mat4_identity();
    m.m[0][0] = scale->x;
    m.m[1][1] = scale->y;
    m.m[2][2] = scale->z;

    return m;
}

mat4_t mat4_make_translation(vec3_t *translation) {
    mat4_t m = mat4_identity();
    m.m[0][3] = translation->x;
    m.m[1][3] = translation->y;
    m.m[2][3] = translation->z;

    return m;
}

mat4_t mat4_make_rotation_x(vec3_t *rotation) {
    mat4_t m = mat4_identity();
    float s = sin(rotation->x);
    float c = cos(rotation->x);

    m.m[1][1] = c;
    m.m[1][2] = s;
    m.m[2][1] = -s;
    m.m[2][2] = c;

    return m;
}

mat4_t mat4_make_rotation_y(vec3_t *rotation) {
    mat4_t m = mat4_identity();
    float s = sin(rotation->y);
    float c = cos(rotation->y);

    m.m[0][0] = c;
    m.m[0][2] = -s;
    m.m[2][0] = s;
    m.m[2][2] = c;

    return m;
}

mat4_t mat4_make_rotation_z(vec3_t *rotation) {
    mat4_t m = mat4_identity();
    float s = sin(rotation->z);
    float c = cos(rotation->z);

    m.m[0][0] = c;
    m.m[0][1] = s;
    m.m[1][0] = -s;
    m.m[1][1] = c;

    return m;
}

vec4_t mat4_mult_vec4(mat4_t *m, vec4_t *v) {
    vec4_t result;
    result.x = m->m[0][0] * v->x + m->m[0][1] * v->y + m->m[0][2] * v->z + m->m[0][3] * v->w;
    result.y = m->m[1][0] * v->x + m->m[1][1] * v->y + m->m[1][2] * v->z + m->m[1][3] * v->w;
    result.z = m->m[2][0] * v->x + m->m[2][1] * v->y + m->m[2][2] * v->z + m->m[2][3] * v->w;
    result.w = m->m[3][0] * v->x + m->m[3][1] * v->y + m->m[3][2] * v->z + m->m[3][3] * v->w;

    return result;
}

mat4_t mat4_mult_mat4(mat4_t *a, mat4_t *b) {
    mat4_t m;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = a->m[i][0] * b->m[0][j] + a->m[i][1] * b->m[1][j] 
                + a->m[i][2] * b->m[2][j] + a->m[i][3] * b->m[3][j];
        }
    }

    return m;
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar) {
    mat4_t m = {{{ 0 }}};
    m.m[0][0] = aspect * (1 / tan(fov / 2));
    m.m[1][1] = 1 / tan(fov / 2);
    m.m[2][2] = zfar / (zfar - znear);
    m.m[2][3] = (-zfar * znear) / (zfar - znear);
    m.m[3][2] = 1.0;

    return m;
}

vec4_t mat4_mult_vec4_project(mat4_t *mat_proj, vec4_t *v) {
    vec4_t result = mat4_mult_vec4(mat_proj, v);

    if (result.w != 0.0) {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }

    return result;
}

mat4_t mat4_look_at(vec3_t *eye, vec3_t *target, vec3_t *up_helper) {
    vec3_t forward = vec3_sub(target, eye);
    vec3_normalize(&forward);

    vec3_t right = vec3_cross(up_helper, &forward);
    vec3_normalize(&right);

    vec3_t up = vec3_cross(&forward, &right);
    vec3_normalize(&up);

    mat4_t view_matrix = {{
        {right.x, right.y, right.z, -vec3_dot(&right, eye)},
        {up.x, up.y, up.z, -vec3_dot(&up, eye)},
        {forward.x, forward.y, forward.z, -vec3_dot(&forward, eye)},
        {0, 0, 0, 1}
    }};

    return view_matrix;
}
