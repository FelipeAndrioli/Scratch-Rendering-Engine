#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include "./vector.h"

typedef uint32_t color_t;

// face_t stores vertex index
typedef struct {
    int a;
    int b;
    int c;
    color_t color;
} face_t;

// triangle_t stores the actual vec2 points of the triangle in the screen
typedef struct {
    vec2_t points[3];
    color_t color;
} triangle_t;

#endif
