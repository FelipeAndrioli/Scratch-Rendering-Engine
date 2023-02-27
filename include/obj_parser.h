#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/dynamic_array/array.h"

#include "./mesh.h"

void load_obj_data(char* filepath);
void process_line(char* line);
void process_char(char* vertex);
void process_face(char* line);
void process_vertex(char* vertex);
void process_normals(char* line);
void process_texture(char* line);

extern tex2_t *uvs;

#endif
