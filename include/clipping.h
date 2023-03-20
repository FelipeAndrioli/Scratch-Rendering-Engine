#ifndef CLIPPING_H
#define CLIPPING_H

#include "./vector.h"

#define NUM_PLANES 6
#define MAX_POLYGON_VERTICES 10

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

typedef struct {
    vec3_t vertices[MAX_POLYGON_VERTICES];
    int num_vertices;
} polygon_t;

void init_frustum_planes(float fov, float z_near, float z_far);
void clip(polygon_t *polygon, int plane);
void clip_polygon(polygon_t *polygon);
polygon_t polygon_from_triangle(vec4_t *triangle);

#endif
