#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#include "./vector.h"
#include "./matrix.h"

void log_const_char(const char* message);
void log_int_value(int value);
void log_double_value(float value);
void log_vec3(vec3_t vec3);
void log_vec3_pointer(vec3_t *vec3);
void log_vec4(vec4_t vec4);
void log_mat4(mat4_t m);

#endif
