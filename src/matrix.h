#ifndef MATRIX_H
#define MATRIX_H

#include "vectors.h"

typedef struct{
    float m[4][4];
}mat4_t;

/**
 * returns the identity matrix in a 4X4 dimension
 * 
 * | 1 0 0 0 |
 * | 0 1 0 0 |
 * | 0 0 1 0 |
 * | 0 0 0 1 |
 */
mat4_t mat4_identity(void);

/**
 * Creates a 4X4 scale matrix.
 * 
 * | sx  0  0  0 | ```
 * |  0 sy  0  0 | 
 * |  0  0  sz 0 | 
 * |  0  0  0  1 |
 * 
 * @param sx the scale factor for the x axis.
 * @param sy the scale factor for the y axis.
 * @param sz the scale factor for the z axis.
 * 
 * @return the 4X4 scale matrix.
 */
mat4_t mat4_make_scale(float sx, float sy, float sz);


/**
 * Creates a 4X4 translation matrix.
 * 
 * |  1  0  0  sx | ```
 * |  0  1  0  sy | 
 * |  0  0  1  sz | 
 * |  0  0  0  1  |
 * 
 * @param tx the translation factor for the x axis.
 * @param ty the translation factor for the y axis.
 * @param tz the translation factor for the z axis.
 * 
 * @return the 4X4 translation matrix.
 */
mat4_t mat4_make_translation(float tx, float ty, float tz);


/**
 * Makes a 4X4 matrix that represents the rotation in the z axis.
 * 
 * |  c  -s  0  0 | ```
 * |  s   c  0  0 | 
 * |  0   0  1  0 | 
 * |  0   0  0  1  |
 * 
 * @param angle the angle to rotate.
 */
mat4_t mat4_make_rotation_z(float angle);

/**
 * Makes a 4X4 matrix that represents the rotation in the x axis.
 * 
 * |  1   0  0  0 | ```
 * |  c  -s  0  0 | 
 * |  s   c  1  0 | 
 * |  0   0  0  1  |
 * 
 * @param angle the angle to rotate.
 */
mat4_t mat4_make_rotation_x(float angle);

/**
 * Makes a 4X4 matrix that represents the rotation in the y axis.
 * 
 * |  c  s  0  0 | ```
 * |  0  1  0  0 | 
 * | -s  c  1  0 | 
 * |  0  0  0  1 |
 * 
 * @param angle the angle to rotate.
 */
mat4_t mat4_make_rotation_y(float angle);

/**
 * Multiply a 4X4 matrix by a 4X1 matrix (which is a vector with 4 dimensions)
 */
vec4_t mat4_mul_vec4(mat4_t mat, vec4_t vec);

#endif