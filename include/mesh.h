#ifndef MESH_H
#define MESH_H

#include <stddef.h>

#include "../libs/dynamic_array/array.h"
#include "../libs/uPNG/upng.h"

#include "./vector.h"
#include "./texture.h"
#include "./triangle.h"

#define MAX_NUM_MESHES 10

typedef struct {
    vec3_t *vertices;
    face_t *faces;
    upng_t *texture;
    vec3_t rotation;
    vec3_t scale;
    vec3_t translation;
} mesh_t;

#include "./obj_parser.h"

void free_meshes(void);
void load_model_mesh_data(mesh_t *mesh, char *filepath);
void load_png_texture_data(mesh_t *mesh, char *filepath);
void load_mesh(char *obj_filepath, char *texture_filepath, vec3_t scale, 
    vec3_t translation, vec3_t rotation);

int get_num_meshes(void);

mesh_t *get_mesh(int mesh_index);

#endif
