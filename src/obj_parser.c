#include "../include/obj_parser.h"

void process_vertex(char* vertex) {
    char *end_str = strdup(vertex);
    char *token = strtok_r(NULL, " ", &end_str);
    // token is initialized this way to skip the first value on the string
    // which is an "v"
    vec3_t new_vertex;

    token = strtok_r(end_str, " ", &end_str);
    new_vertex.x = atof(token);
    token = strtok_r(end_str, " ", &end_str);
    new_vertex.y = atof(token);
    token = strtok_r(end_str, " ", &end_str);
    new_vertex.z = atof(token);

    array_push(mesh.vertices, new_vertex);
}

void process_normals(char* line) {
    char* end_str = strdup(line);
    char* token = strtok_r(NULL, " ", &end_str);

    vec3_t new_normal;

    token = strtok_r(end_str, " ", &end_str);
    new_normal.x = atof(token);
    token = strtok_r(end_str, " ", &end_str);
    new_normal.y = atof(token);
    token = strtok_r(end_str, " ", &end_str);
    new_normal.z = atof(token);

    array_push(mesh.normals, new_normal);
}

void process_face(char *face) {
    /*
        vi -> vertex indice
        ti -> texture indice
        ni -> normal indice

        data structure
        vi vi vi
        ti ti ti
        ni ni ni
    */

    char *end_str = strdup(face);
    char *token = strtok_r(NULL, " ", &end_str);
    // token is initialized this way to skip the first value on the string
    // which is an "f"
    face_t new_face;
    int data[3][3];
    int helper = 0;

    // 1/2/3/ 4/5/6 7/8/9
    while ((token = strtok_r(end_str, " ", &end_str))) {
        // 1/2/3
        char *sub_end_str = strdup(token);

        for (int i = 0; i < 3; i++) {
            char *sub_token = strtok_r(sub_end_str, "/", &sub_end_str);
            data[i][helper] = atoi(sub_token); 
        }
        helper++;
    }

    new_face.a = data[0][0];
    new_face.b = data[0][1];
    new_face.c = data[0][2];

    new_face.na = data[2][0];
    new_face.nb = data[2][1];
    new_face.nc = data[2][2];

    array_push(mesh.faces, new_face);
}

void process_line(char* line) {
    char *end_str = strdup(line);
    char *token = strtok_r(end_str, " ", &end_str);

    /*
        strdup - allocate memory on heap and copy the value on memory from the
        orinal string to the new one, therefore it won't mess the original
        string
    */

    if (!strcmp(token, "v")) {
        process_vertex(line);
    }

    if (!strcmp(token, "f")) {
        process_face(line);
    }

    if (!strcmp(token, "n")) {
        process_normals(line);
    }
}

void load_obj_data(char* path) {
    FILE *fp;
    char buff[255];

    if((fp = fopen(path, "r")) == NULL) {
        fprintf(stderr, "Error opening file");
        return;
    }

    while (fgets(buff, 255, fp) != NULL) {
        process_line(buff);
    }

    fclose(fp);
}
