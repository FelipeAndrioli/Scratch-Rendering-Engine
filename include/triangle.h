#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

#include "../libs/uPNG/upng.h"

#include "./vector.h"
#include "./texture.h"

typedef uint32_t color_t;

// face_t stores vertex index
typedef struct {
    int a;
    int b;
    int c;
    /*
    int na;
    int nb;
    int nc;
    */

    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;
    color_t color;
} face_t;

// triangle_t stores the actual vec2 points of the triangle in the screen
typedef struct {
    vec4_t points[3];
    tex2_t texcoords[3];
    color_t color;
    upng_t *texture;
} triangle_t;

vec3_t calculate_triangle_normal(vec4_t vertices[3]); 

#endif
