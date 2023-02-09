#include "../include/mesh.h"

mesh_t mesh = {
    NULL, 
    NULL, 
    NULL,
    {0, 0, 0},          // rotation
    {1.0, 1.0, 1.0},    // scale
    {0.0, 0.0, 0.0}     // translation
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {-1, -1, -1},
    {-1, 1, -1},
    {1, 1, -1},
    {1, -1, -1},
    {1, 1, 1},
    {1, -1, 1},
    {-1 ,1 ,1},
    {-1 , -1, 1}
};

face_t cube_faces[N_CUBE_FACES] = {
    {1, 2, 3, {0, 0}, {0, 1}, {1, 1}, 0xFFFF0000},
    {1, 3, 4, {0, 0}, {1, 1}, {1, 0}, 0xFFFF0000},
    {4, 3, 5, {0, 0}, {0, 1}, {1, 1}, 0xFF00FF00},
    {4, 5, 6, {0, 0}, {1, 1}, {1, 0}, 0xFF00FF00},
    {6, 5, 7, {0, 0}, {0, 1}, {1, 1}, 0xFF0000FF},
    {6, 7, 8, {0, 0}, {1, 1}, {1, 0}, 0xFF0000FF},
    {8, 7, 2, {0, 0}, {0, 1}, {1, 1}, 0xFFFF00FF},
    {8, 2, 1, {0, 0}, {1, 1}, {1, 0}, 0xFFFF00FF},
    {2, 7, 5, {0, 0}, {0, 1}, {1, 1}, 0xFFFFFF00},
    {2, 5, 3, {0, 0}, {1, 1}, {1, 0}, 0xFFFFFF00},
    {6, 8, 1, {0, 0}, {0, 1}, {1, 1}, 0xFF00FFFF},
    {6, 1, 4, {0, 0}, {1, 1}, {1, 0}, 0xFF00FFFF}
};

void load_cube_mesh_data(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }

    for (int i = 0; i < N_CUBE_FACES; i++) {
        face_t cube_face = cube_faces[i];
        array_push(mesh.faces, cube_face);
    }
}

void load_model_mesh_data(char* filepath) {
    load_obj_data(filepath);
}
