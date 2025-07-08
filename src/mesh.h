#ifndef MESH_H
#define MESH_H

#include "vectors.h"
#include "triangle.h"
#include "upng.h"

/**
 * type that contains an array of vertices and an array of faces of an object as well as its rotation and translation values
 */
typedef struct{
    /** array of vertices */
    vec3_t* vertices;
    
    /** array of faces */
    face_t* faces;

    /** texture associated with the mesh */
    upng_t* texture;

    /** rotation of the object  with x, y and z values*/
    vec3_t rotation;

    /** the scale of the object  with x, y and z values*/
    vec3_t scale;

    /** the translation of the object with x, y and z values */
    vec3_t translation;
}mesh_t;


/**
 * Initializes a new mesh.
 */
void load_mesh(char* mesh_path, char* texture_path, vec3_t scale, vec3_t translation, vec3_t rotation);

/**
 * loads an external obj file
 * \param filename the path of the file to be loaded
 */
void load_mesh_obj_data(char* filename, mesh_t* mesh);

void load_obj_texture_data(char *filename, mesh_t* mesh);

int get_num_meshes(void);
mesh_t* get_mesh_at(int index);
void free_meshes(void);

#endif