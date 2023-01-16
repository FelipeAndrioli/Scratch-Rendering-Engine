#include "../include/obj_parser.h"

void process_vertex(char* vertex) {
    while (vertex) {
        // convert and add to the array
        printf("%f\n", atof(vertex));
        vertex = strtok(NULL, " ");
    }
}

void process_face(char *face) {
    // process how many set of data splitted by " " is provided
    while (face) {
        // extract values, convert and add to the array

        char* helper = strtok(face, "/");

        // it'll always going to be a set a of three values splitted between /
        int vertex_indice = 0;
        int texture_indice = 0;
        int normal_indice = 0;

        vertex_indice = atoi(helper);
        helper = strtok(NULL, "/");

        texture_indice = atoi(helper);
        helper = strtok(NULL, "/");

        normal_indice = atoi(helper);
        helper = strtok(NULL, "/");

        face = strtok(NULL, " ");
    }
}

void process_line(char* line) {
    int index = 0;

    if (!strcmp(line, "v")) {
        line = strtok(NULL, " ");
        process_vertex(line);
    }

    if (!strcmp(line, "f")) {
        line = strtok(NULL, " ");
        process_face(line);
    }
}

void load_obj_data(char* path) {

    /*

       fopen needs to be tested

       the function fgets allows to read string from a stream. It reads up to
       n-1 characters from the input stream referenced by fp. It copies the
       read string into the buffer buf,m appending a null character to 
       terminate the string.

    */

    FILE *fp;
    char buff[255];

    if((fp = fopen(path, "r")) == NULL) {
        fprintf(stderr, "Error opening file");
        return;
    }

    fscanf(fp, "%s", buff);
    while (fgets(buff, 255, fp) != NULL) {
        char* helper;
        helper = strtok(buff, " ");
        process_line(buff);
    }

    fclose(fp);
}
