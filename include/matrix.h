#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>

#include "./vector.h"

typedef struct {
    float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_make_scale(vec3_t *scale);
mat4_t mat4_make_translation(vec3_t *translation);
mat4_t mat4_make_rotation_x(vec3_t *rotation);
mat4_t mat4_make_rotation_y(vec3_t *rotation);
mat4_t mat4_make_rotation_z(vec3_t *rotation);
vec4_t mat4_mult_vec4(mat4_t *m, vec4_t *v);
vec4_t mat4_mult_vec4_dir(mat4_t *m, vec4_t *v);
mat4_t mat4_mult_mat4(mat4_t *a, mat4_t *b);
mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);
vec4_t mat4_mult_vec4_project(mat4_t *mat_proj, vec4_t *v);

mat4_t mat4_transpose(mat4_t *m);
mat4_t mat4_inverse(mat4_t *m);

#endif
