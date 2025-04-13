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

vec4_t mat4_mul_vec4(mat4_t scale_matrix, vec4_t vec);

#endif