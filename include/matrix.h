#ifndef MATRIX_H
#define MATRIX_H

#include "./vector.h"

typedef struct {
    float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_make_scale(vec3_t *scale);
mat4_t mat4_make_translation(vec3_t *translation);
vec4_t mat4_mult_vec4(mat4_t m, vec4_t v);

#endif
