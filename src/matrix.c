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

vec4_t mat4_mult_vec4_dir(mat4_t *m, vec4_t *v) {
    vec4_t result;

    result.x = m->m[0][0] * v->x + m->m[0][1] * v->y + m->m[0][2] * v->z + m->m[0][3] * v->w;
    result.y = m->m[1][0] * v->x + m->m[1][1] * v->y + m->m[1][2] * v->z + m->m[1][3] * v->w;
    result.z = m->m[2][0] * v->x + m->m[2][1] * v->y + m->m[2][2] * v->z + m->m[2][3] * v->w;
    result.w = 0.0;

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

mat4_t mat4_transpose(mat4_t *m) {
    mat4_t transposeMatrix = {{{ 0 }}};

    transposeMatrix.m[0][0] = m->m[0][0];
    transposeMatrix.m[0][1] = m->m[1][0];
    transposeMatrix.m[0][2] = m->m[2][0];
    transposeMatrix.m[0][3] = m->m[3][0];

    transposeMatrix.m[1][0] = m->m[0][1];
    transposeMatrix.m[1][1] = m->m[1][1];
    transposeMatrix.m[1][2] = m->m[2][1];
    transposeMatrix.m[1][3] = m->m[3][1];
    
    transposeMatrix.m[2][0] = m->m[0][2];
    transposeMatrix.m[2][1] = m->m[1][2];
    transposeMatrix.m[2][2] = m->m[2][2];
    transposeMatrix.m[2][3] = m->m[3][2];
    
    transposeMatrix.m[3][0] = m->m[0][3];
    transposeMatrix.m[3][1] = m->m[1][3];
    transposeMatrix.m[3][2] = m->m[2][3];
    transposeMatrix.m[3][3] = m->m[3][3];

    return transposeMatrix;
}

mat4_t mat4_inverse(mat4_t *m) {
    mat4_t inverseMatrix = {{{ 0 }}};
    
    float 
        a00 = m->m[0][0], a01 = m->m[0][1], a02 = m->m[0][2], a03 = m->m[0][3],
        a10 = m->m[1][0], a11 = m->m[1][1], a12 = m->m[1][2], a13 = m->m[1][3],
        a20 = m->m[2][0], a21 = m->m[2][1], a22 = m->m[2][2], a23 = m->m[2][3],
        a30 = m->m[3][0], a31 = m->m[3][1], a32 = m->m[3][2], a33 = m->m[3][3],

        b00 = a00 * a11 - a01 * a10,
        b01 = a00 * a12 - a02 * a10,
        b02 = a00 * a13 - a03 * a10,
        b03 = a01 * a12 - a02 * a11,
        b04 = a01 * a13 - a03 * a11,
        b05 = a02 * a13 - a03 * a12,
        b06 = a20 * a31 - a21 * a30,
        b07 = a20 * a32 - a22 * a30,
        b08 = a20 * a33 - a23 * a30,
        b09 = a21 * a32 - a22 * a31,
        b10 = a21 * a33 - a23 * a31,
        b11 = a22 * a33 - a23 * a32;

    float determinant = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
    determinant = 1 / determinant;
   
    inverseMatrix.m[0][0] = (a11 * b11 - a12 * b10 + a13 * b09) * determinant;
    inverseMatrix.m[1][0] = (a02 * b10 - a01 * b11 - a03 * b09) * determinant;
    inverseMatrix.m[2][0] = (a31 * b05 - a32 * b04 + a33 * b03) * determinant;
    inverseMatrix.m[3][0] = (a22 * b04 - a21 * b05 - a23 * b03) * determinant;
      
    inverseMatrix.m[0][1] = (a12 * b08 - a10 * b11 - a13 * b07) * determinant;
    inverseMatrix.m[1][1] = (a00 * b11 - a02 * b08 + a03 * b07) * determinant;
    inverseMatrix.m[2][1] = (a32 * b02 - a30 * b05 - a33 * b01) * determinant;
    inverseMatrix.m[3][1] = (a20 * b05 - a22 * b02 + a23 * b01) * determinant;

    inverseMatrix.m[0][2] = (a10 * b10 - a11 * b08 + a13 * b06) * determinant;
    inverseMatrix.m[1][2] = (a01 * b08 - a00 * b10 - a03 * b06) * determinant;
    inverseMatrix.m[2][2] = (a30 * b04 - a31 * b02 + a33 * b00) * determinant;
    inverseMatrix.m[3][2] = (a21 * b02 - a20 * b04 - a23 * b00) * determinant;
      
    inverseMatrix.m[0][3] = (a11 * b07 - a10 * b09 - a12 * b06) * determinant;
    inverseMatrix.m[1][3] = (a00 * b09 - a01 * b07 + a02 * b06) * determinant;
    inverseMatrix.m[2][3] = (a31 * b01 - a30 * b03 - a32 * b00) * determinant;
    inverseMatrix.m[3][3] = (a20 * b03 - a21 * b01 + a22 * b00) * determinant;

    return inverseMatrix;
}

