#ifndef MESH_H
#define MESH_H

#include "vectors.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2)

extern vec3_t mesh_vertices[N_CUBE_VERTICES];
extern face_t mesh_faces[N_CUBE_FACES];

/**
 * type that contains an array of vertices and an array of faces of an object as well as its rotation and translation values
 */
typedef struct{
    /** array of vertices */
    vec3_t* vertices;
    
    /** array of faces */
    face_t* faces;

    /** rotation of the object  with x, y and z values*/
    vec3_t rotation;

    /** the scale of the object  with x, y and z values*/
    vec3_t scale;

    /** the translation of the object with x, y and z values */
    vec3_t translation;
}mesh_t;

extern mesh_t mesh;

/**
 * loads the cube values in the mesh data structure
 */
void load_cube_mesh_data(void);

/**
 * loads an external obj file
 * \param filename the path of the file to be loaded
 */
void load_obj_mesh_data(char* filename);

#endif