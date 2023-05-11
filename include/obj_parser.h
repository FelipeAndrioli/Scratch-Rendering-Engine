#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/dynamic_array/array.h"

#include "./texture.h"
#include "./mesh.h"
#include "./log.h"

void load_obj_data(mesh_t *mesh, char *filepath);
void process_line(mesh_t *mesh, char *line);
void process_char(char* vertex);
void process_face(mesh_t *mesh, char *line);
void process_vertex(mesh_t *mesh, char *vertex);
void process_texture(char *line);

extern tex2_t *uvs;
extern vec3_t *normals;

#endif
