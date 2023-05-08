#ifndef CLIPPING_H
#define CLIPPING_H

#include "./vector.h"
#include "./triangle.h"
#include "./log.h"

#define NUM_PLANES 6
#define MAX_POLYGON_VERTICES 10
#define MAX_CLIPPED_TRIANGLES 10

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
    vec3_t normals[MAX_POLYGON_VERTICES];
    tex2_t texcoords[MAX_POLYGON_VERTICES];
    int num_vertices;
} polygon_t;

void init_frustum_planes(float fov_x, float fov_y, float z_near, float z_far);
float float_lerp(float a, float b, float t);
void clip(polygon_t *polygon, int plane);
void clip_polygon(polygon_t *polygon);
void triangles_from_polygon(polygon_t *polygon, triangle_t triangles[], int *n_clipped_triangles);
polygon_t polygon_from_triangle(vec4_t *triangle, vec4_t *normals, tex2_t t0, tex2_t t1, tex2_t t2);

#endif
