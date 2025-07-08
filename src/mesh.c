#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "mesh.h"

#define MAX_NUM_MESHES 10
static mesh_t meshes[MAX_NUM_MESHES];
static int mesh_count = 0;

void load_mesh(char *mesh_path, char *texture_path, vec3_t scale, vec3_t translation, vec3_t rotation)
{
    mesh_t mesh = {
        .scale = scale, 
        .translation = translation, 
        .rotation = rotation
    };

    load_mesh_obj_data(mesh_path, &mesh);
    load_obj_texture_data(texture_path, &mesh);

    meshes[mesh_count] = mesh;
    mesh_count++;
}


void load_mesh_obj_data(char* filename, mesh_t* mesh){

    FILE *file = fopen(filename, "r");

    if(file == NULL){
        printf("file is null\n");
        fclose(file);
        return;
    }

    tex2_t* texcoords = NULL;

    char line[1024];
    while(fgets(line, 1024, file)){
        // Vertices information
        if(strncmp(line, "v ", 2) == 0){
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(mesh->vertices, vertex);
        }

        if(strncmp(line, "vt ", 3) == 0){
            tex2_t texcoord;
            sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);
            array_push(texcoords, texcoord);
        }
        
        // Faces information
        if(strncmp(line, "f ", 2) == 0){
            int vertex_indices[3];
            int textures_indices[3];
            int normals_indices[3];
            
            sscanf(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                &vertex_indices[0], &textures_indices[0], &normals_indices[0],
                &vertex_indices[1], &textures_indices[1], &normals_indices[1],
                &vertex_indices[2], &textures_indices[2], &normals_indices[2]
            );

            face_t face = {
                // The indices on obj files starts at 1, thats why its subtracting 1.

                .a = vertex_indices[0] - 1,
                .b = vertex_indices[1] - 1,
                .c = vertex_indices[2] - 1,

                .a_uv = texcoords[textures_indices[0] - 1],
                .b_uv = texcoords[textures_indices[1] - 1],
                .c_uv = texcoords[textures_indices[2] - 1],

                .color = 0xFFFFFFFF
            };

            array_push(mesh->faces, face);
        }

    }
    
    array_free(texcoords);
    fclose(file);
}

void load_obj_texture_data(char *filename, mesh_t* mesh)
{
    upng_t* png_image = upng_new_from_file(filename);

    if(png_image != NULL){
        upng_decode(png_image);

        if(upng_get_error(png_image) == UPNG_EOK){
            mesh->texture = png_image;
        }
    }
}

int get_num_meshes(void)
{
    return mesh_count;
}

mesh_t* get_mesh_at(int index)
{
    if(index < 0 || index >= MAX_NUM_MESHES){
        return &meshes[0];
    }

    return &meshes[index];
}

void free_meshes(void)
{
    for(int i = 0 ; i < mesh_count; i++){
        mesh_t* mesh = &meshes[i];

        upng_free(mesh->texture);
        array_free(mesh->faces);
	    array_free(mesh->vertices);
    }
}
