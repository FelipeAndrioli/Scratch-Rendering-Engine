#include "../include/clipping.h"

plane_t frustum_planes[NUM_PLANES];

void init_frustum_planes(float fov, float z_near, float z_far) {

    float cos_half_fov = cos(fov / 2);
    float sin_half_fov = sin(fov / 2);

    vec3_t origin = vec3_new(0, 0, 0);

    frustum_planes[LEFT_PLANE].point = origin;
    frustum_planes[LEFT_PLANE].normal.x = cos_half_fov;
    frustum_planes[LEFT_PLANE].normal.y = 0;
    frustum_planes[LEFT_PLANE].normal.z = sin_half_fov;
    
    frustum_planes[RIGHT_PLANE].point = origin;
    frustum_planes[RIGHT_PLANE].normal.x = -cos_half_fov;
    frustum_planes[RIGHT_PLANE].normal.y = 0;
    frustum_planes[RIGHT_PLANE].normal.z = sin_half_fov;

    frustum_planes[TOP_PLANE].point = origin;
    frustum_planes[TOP_PLANE].normal.x = 0;
    frustum_planes[TOP_PLANE].normal.y = -cos_half_fov;
    frustum_planes[TOP_PLANE].normal.z = sin_half_fov;
    
    frustum_planes[BOTTOM_PLANE].point = origin;
    frustum_planes[BOTTOM_PLANE].normal.x = 0;
    frustum_planes[BOTTOM_PLANE].normal.y = cos_half_fov;
    frustum_planes[BOTTOM_PLANE].normal.z = sin_half_fov;
    
    frustum_planes[NEAR_PLANE].point = vec3_new(0, 0, z_near);
    frustum_planes[NEAR_PLANE].normal.x = 0;
    frustum_planes[NEAR_PLANE].normal.y = 0;
    frustum_planes[NEAR_PLANE].normal.z = 1;
    
    frustum_planes[FAR_PLANE].point = vec3_new(0, 0, z_far);
    frustum_planes[FAR_PLANE].normal.x = 0;
    frustum_planes[FAR_PLANE].normal.y = 0;
    frustum_planes[FAR_PLANE].normal.z = -1;
}

void clip(polygon_t *polygon, int plane) {
    vec3_t plane_point = frustum_planes[plane].point;
    vec3_t plane_normal = frustum_planes[plane].normal;

    vec3_t inside_vertices[MAX_POLYGON_VERTICES];
    int n_inside_vertices = 0;

    vec3_t *current_vertex = &polygon->vertices[0];
    vec3_t *previous_vertex = &polygon->vertices[polygon->num_vertices - 1];

    vec3_t current_point = vec3_sub(current_vertex, &plane_point);
    float dot_current_vertex = 0;

    vec3_t previous_point = vec3_sub(previous_vertex, &plane_point);
    float dot_previous_vertex = vec3_dot(&previous_point, &plane_normal);

    while (current_vertex != &polygon->vertices[polygon->num_vertices]) {
        current_point = vec3_sub(current_vertex, &plane_point);
        dot_current_vertex = vec3_dot(&current_point, &plane_normal);

        // change the state of the vertex (inside -> outside or vice versa)
        if (dot_current_vertex * dot_previous_vertex < 0) {
            // calculate the t factor 
            float t = dot_previous_vertex / (dot_previous_vertex - dot_current_vertex);

            // calculate the intersection point
            vec3_t intersection_point = vec3_clone(current_vertex);
            intersection_point = vec3_sub(&intersection_point, previous_vertex);
            intersection_point = vec3_mult(&intersection_point, t);
            intersection_point = vec3_add(&intersection_point, previous_vertex);

            inside_vertices[n_inside_vertices] = vec3_clone(&intersection_point);
            n_inside_vertices++;
        }

        // vertex inside the plane
        if (dot_current_vertex > 0) {
            inside_vertices[n_inside_vertices] = vec3_clone(current_vertex);
            n_inside_vertices++;
        }

        dot_previous_vertex = dot_current_vertex; 
        previous_vertex = current_vertex;
        current_vertex++;
    }

    for (int i = 0; i < n_inside_vertices; i++) {
        polygon->vertices[i] = vec3_clone(&inside_vertices[i]);
    }
    polygon->num_vertices = n_inside_vertices;
}

void clip_polygon(polygon_t *polygon) {
    clip(polygon, LEFT_PLANE);
    clip(polygon, RIGHT_PLANE);
    clip(polygon, TOP_PLANE);
    clip(polygon, BOTTOM_PLANE);
    clip(polygon, NEAR_PLANE);
    clip(polygon, FAR_PLANE);
}

void triangles_from_polygon(polygon_t *polygon, triangle_t triangles[], 
    int *n_clipped_triangles) {

    for (int i = 0; i < polygon->num_vertices - 2; i++) {
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;

        triangles[i].points[0] = vec4_from_vec3(polygon->vertices[index0]);
        triangles[i].points[1] = vec4_from_vec3(polygon->vertices[index1]);
        triangles[i].points[2] = vec4_from_vec3(polygon->vertices[index2]);
    }
    *n_clipped_triangles = polygon->num_vertices - 2;
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
