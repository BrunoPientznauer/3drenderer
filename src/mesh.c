#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "mesh.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0, 0, 0}
};

vec3_t mesh_vertices[N_CUBE_VERTICES] = {
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }  // 8
};

face_t mesh_faces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3, .color = 0XFFFF0000},
    {.a = 1, .b = 3, .c = 4, .color = 0XFFFF0000},
    // right
    {.a = 4, .b = 3, .c = 5, .color = 0XFF00FF00},
    {.a = 4, .b = 5, .c = 6, .color = 0XFF00FF00},
    // back
    {.a = 6, .b = 5, .c = 7, .color = 0XFF0000FF},
    {.a = 6, .b = 7, .c = 8, .color = 0XFF0000FF},
    // left
    {.a = 8, .b = 7, .c = 2, .color = 0XFF00FFFF},
    {.a = 8, .b = 2, .c = 1, .color = 0XFF00FFFF},
    // top
    {.a = 2, .b = 7, .c = 5, .color = 0XFFFF00FF},
    {.a = 2, .b = 5, .c = 3, .color = 0XFFFF00FF},
    // bottom
    {.a = 6, .b = 8, .c = 1, .color = 0XFF00FFFF},
    {.a = 6, .b = 1, .c = 4, .color = 0XFF00FFFF}
};


void load_cube_mesh_data(void){
    for(int i = 0; i < N_CUBE_VERTICES; i++){
        vec3_t vertex = mesh_vertices[i];
        array_push(mesh.vertices, vertex);
    }

    for(int i = 0; i < N_CUBE_FACES; i++){
        face_t face = mesh_faces[i];
        array_push(mesh.faces, face);
    }
}

void load_obj_mesh_data(char* filename){

    FILE *file = fopen(filename, "r");

    if(file == NULL){
        printf("file is null\n");
        fclose(file);
        return;
    }

    char line[1024];
    while(fgets(line, 1024, file)){
        // Vertices information
        if(strncmp(line, "v ", 2) == 0){
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(mesh.vertices, vertex);
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

            face_t face = {vertex_indices[0], vertex_indices[1], vertex_indices[2]};
            array_push(mesh.faces, face);

            // printf("{%d, %d ,%d}\n", face.a, face.b, face.c);
        }
    }

    fclose(file);
}