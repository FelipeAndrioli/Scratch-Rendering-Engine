#ifndef CLIPPING_H
#define CLIPPING_H

#include "./vector.h"

enum {
    LEFT_PLANE,
    RIGHT_PLANE,
    TOP_PLANE,
    BOTTOM_PLANE,
    NEAR_PLANE,
    FAR_PLANE
};

typedef struct {
    vec3_t point;
    vec3_t normal;
} plane_t;

#endif
