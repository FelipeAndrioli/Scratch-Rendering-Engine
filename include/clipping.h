#ifndef CLIPPING_H
#define CLIPPING_H

#include "./vector.h"

#define NUM_PLANES 6

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

void init_frustum_planes(float fov, float z_near, float z_far);

#endif
