#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdio.h>
#include <stdlib.h>

void load_obj_data(char* filepath);
void process_line(char* line);
void process_char(char* vertex);
void process_face(char* line);

#endif
