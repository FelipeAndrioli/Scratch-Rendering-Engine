#ifndef MESH_H
#define MESH_H

#include <stddef.h>

#include "../libs/dynamic_array/array.h"

#include "./vector.h"
#include "./triangle.h"
#include "./obj_parser.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

typedef struct {
    vec3_t *vertices;
    face_t *faces;
    vec3_t rotation;
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_model_mesh_data(char* filepath);

#endif
