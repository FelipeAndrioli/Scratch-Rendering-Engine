#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#include "./vector.h"

void log_const_char(const char* message);
void log_int_value(int value);
void log_double_value(float value);
void log_vec3(vec3_t vec3);

#endif
