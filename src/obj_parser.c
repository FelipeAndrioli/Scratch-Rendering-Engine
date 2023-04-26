#include "../include/obj_parser.h"

tex2_t *uvs = NULL;
vec3_t *normals = NULL;

void process_vertex(mesh_t *mesh, char* vertex) {
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

    array_push(mesh->vertices, new_vertex);
}

void process_texture(char *line) {
    char *end_str = strdup(line);
    char *token = strtok_r(NULL, " ", &end_str);

    tex2_t new_texture;
    token = strtok_r(end_str, " ", &end_str);
    new_texture.u = atof(token);
    token = strtok_r(end_str, " ", &end_str);
    new_texture.v = atof(token);

    array_push(uvs, new_texture);
}

void process_normals(char *line) {
    char *end_str = strdup(line);
    char *token = strtok_r(NULL, " ", &end_str);

    vec3_t new_normal;

    token = strtok_r(end_str, " ", &end_str);
    new_normal.x = atof(token);
    token = strtok_r(end_str, " ", &end_str);
    new_normal.y = atof(token);
    token = strtok_r(end_str, " ", &end_str);
    new_normal.z = atof(token);

    array_push(normals, new_normal);
}

void process_face(mesh_t *mesh, char *face) {
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

    new_face.na = normals[data[2][0] - 1];
    new_face.nb = normals[data[2][1] - 1];
    new_face.nc = normals[data[2][2] - 1];

    new_face.a_uv = uvs[data[1][0] - 1];
    new_face.b_uv = uvs[data[1][1] - 1];
    new_face.c_uv = uvs[data[1][2] - 1];

    array_push(mesh->faces, new_face);
}

void process_line(mesh_t *mesh, char* line) {
    char *end_str = strdup(line);
    char *token = strtok_r(end_str, " ", &end_str);

    /*
        strdup - allocate memory on heap and copy the value on memory from the
        orinal string to the new one, therefore it won't mess the original
        string
    */

    if (!strcmp(token, "v")) {
        process_vertex(mesh, line);
    }

    if (!strcmp(token, "f")) {
        process_face(mesh, line);
    }

    if (!strcmp(token, "vn")) {
        process_normals(line);
    }

    if (!strcmp(token, "vt")) {
        process_texture(line);
    }
}

void load_obj_data(mesh_t *mesh, char *filepath) {
    FILE *fp;
    char buff[255];

    if((fp = fopen(filepath, "r")) == NULL) {
        fprintf(stderr, "Error opening file");
        return;
    }

    while (fgets(buff, 255, fp) != NULL) {
        process_line(mesh, buff);
    }

    uvs = NULL;
    normals = NULL;

    fclose(fp);
}
