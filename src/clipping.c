#include "../include/clipping.h"


plane_t frustum_planes[NUM_PLANES];

void init_frustum_planes(float fov, float z_near, float z_far) {

    float cos_half_fov = cos(fov / 2);
    float sin_half_fov = sin(fov / 2);

    vec3_t origin = vec3_new(0, 0, 0);

    frustum_planes[LEFT_PLANE].point = origin;
    frustum_planes[LEFT_PLANE].normal.x = cos_half_fov;
    frustum_planes[LEFT_PLANE].normal.y = 0;
    frustum_planes[LEFT_PLANE].normal.x = sin_half_fov;
    
    frustum_planes[RIGHT_PLANE].point = origin;
    frustum_planes[RIGHT_PLANE].normal.x = -cos_half_fov;
    frustum_planes[RIGHT_PLANE].normal.y = 0;
    frustum_planes[RIGHT_PLANE].normal.x = sin_half_fov;

    frustum_planes[TOP_PLANE].point = origin;
    frustum_planes[TOP_PLANE].normal.x = 0;
    frustum_planes[TOP_PLANE].normal.y = -cos_half_fov;
    frustum_planes[TOP_PLANE].normal.x = sin_half_fov;
    
    frustum_planes[BOTTOM_PLANE].point = origin;
    frustum_planes[BOTTOM_PLANE].normal.x = 0;
    frustum_planes[BOTTOM_PLANE].normal.y = cos_half_fov;
    frustum_planes[BOTTOM_PLANE].normal.x = sin_half_fov;
    
    frustum_planes[NEAR_PLANE].point = vec3_new(0, 0, z_near);
    frustum_planes[NEAR_PLANE].normal.x = 0;
    frustum_planes[NEAR_PLANE].normal.y = 0;
    frustum_planes[NEAR_PLANE].normal.x = 1;
    
    frustum_planes[FAR_PLANE].point = vec3_new(0, 0, z_far);
    frustum_planes[FAR_PLANE].normal.x = 0;
    frustum_planes[FAR_PLANE].normal.y = 0;
    frustum_planes[FAR_PLANE].normal.x = -1;
}

void clip(polygon_t *polygon, int plane) {

}

void clip_polygon(polygon_t *polygon) {
    clip(polygon, LEFT_PLANE);
    clip(polygon, RIGHT_PLANE);
    clip(polygon, TOP_PLANE);
    clip(polygon, BOTTOM_PLANE);
    clip(polygon, NEAR_PLANE);
    clip(polygon, FAR_PLANE);
}

polygon_t polygon_from_triangle(vec4_t *triangle) {
    polygon_t polygon = {
        {
            vec3_from_vec4(triangle[0]), 
            vec3_from_vec4(triangle[1]), 
            vec3_from_vec4(triangle[2])
        },
        3
    };

    return polygon;
}
