#include "../include/log.h"

void log_const_char(const char* message) {
    printf("%s\n", message);
}

void log_int_value(int value) {
    printf("%d\n", value);
}

void log_float_value(float value) {
    printf("%f\n", value);
}

void log_vec3(vec3_t vec3) {
    printf("%f %f %f\n", vec3.x, vec3.y, vec3.z);
}
