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

void log_vec3_pointer(vec3_t *vec3) {
    printf("%f %f %f\n", vec3->x, vec3->y, vec3->z);
}

void log_vec4(vec4_t vec4) {
    printf("%f %f %f %f\n", vec4.x, vec4.y, vec4.z, vec4.w);
}

void log_mat4(mat4_t m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", m.m[i][j]);
        }
        printf("\n");
    }
}
