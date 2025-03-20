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
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},
    // right
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},
    // back
    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},
    // left
    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},
    // top
    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},
    // bottom
    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4}
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

    char line[100];
    while(fgets(line, 100, file)){
        // gets the prefix of the line
        char* token = strtok(line, "  ");

        // enters if the prefix correspond to a vertex
        if(strcmp(token, "v") == 0){
            float vertices[3];

            int i = 0;
            while (token != NULL)
            {
                token = strtok(NULL, "  ");

                if(token != NULL){
                   vertices[i++] = atof(token);
                }
            }

            vec3_t vertex = {vertices[0], vertices[1], vertices[2]};
            array_push(mesh.vertices, vertex);

            // printf("{%f, %f, %f}\n", vertex.x, vertex.y, vertex.z);
        }else if(strcmp(token, "f") == 0){
            int faces[3];
            
            int i = 0;
            while(token != NULL){
                token = strtok(NULL, " ");
                
                if(token != NULL){
                    faces[i++] = atoi(&token[0]);
                }
            }

            face_t face = {faces[0], faces[1], faces[2]};
            array_push(mesh.faces, face);

            // printf("{%d, %d, %d}\n", face.a, face.b, face.c);
        }
    }

    fclose(file);
}