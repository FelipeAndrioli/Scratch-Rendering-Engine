#include "../include/mesh.h"

static mesh_t meshes[MAX_NUM_MESHES];
static int mesh_count = 0;

void free_meshes(void) {
    for (int i = 0; i < mesh_count; i++) {
        if (meshes[i].texture != NULL) upng_free(meshes[i].texture);
        array_free(meshes[i].faces);
        array_free(meshes[i].vertices);
    }
}

void load_model_mesh_data(mesh_t *mesh, char *filepath) {
    load_obj_data(mesh, filepath);
}

void load_png_texture_data(mesh_t *mesh, char *filepath) {
    upng_t *png_image = upng_new_from_file(filepath);

    if (png_image != NULL) {
        upng_decode(png_image);
        if (upng_get_error(png_image) == UPNG_EOK) {
            mesh->texture = png_image;
        }
    }
}

void load_mesh(char *obj_filepath, char *texture_filepath, vec3_t scale, 
    vec3_t translation, vec3_t rotation) {

    load_model_mesh_data(&meshes[mesh_count], obj_filepath);
    meshes[mesh_count].scale = scale;
    meshes[mesh_count].translation = translation;
    meshes[mesh_count].rotation = rotation;

    if (texture_filepath != NULL) {
        load_png_texture_data(&meshes[mesh_count], texture_filepath);
    }

    mesh_count++;
}

int get_num_meshes(void) {
    return mesh_count;
}

mesh_t *get_mesh(int mesh_index) {
    return &meshes[mesh_index];
}
