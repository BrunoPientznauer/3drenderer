#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>

typedef struct{
    float x, y;
} vec2_t;

typedef struct{
    float x, y, z;
} vec3_t;

/* Vector 2D functions*/

/** returns the magnitude of a 2 dimensional vector */
float vec2_length(vec2_t v);

/** returns the resultant vector of an addition operation of two vectors */
vec2_t vec2_add(vec2_t v1, vec2_t v2);

/** returns the resultant vector of an subtraction operation of two vectors */
vec2_t vec2_sub(vec2_t v1, vec2_t v2);

/** returns the resultant vector of an multiply operation between a vector and a scalar factor */
vec2_t vec2_mul(vec2_t v, float factor);

/** returns the resultant vector of an division operation between a vector and a scalar factor */
vec2_t vec2_div(vec2_t v, float factor);

/** returns the resultant scalar value of a dot product operation between two vectors */
float vec2_dot(vec3_t v1, vec3_t v2);

/* Vector 3D functions*/

/**  returns the maginute of a 3 dimensional vector */
float vec3_length(vec3_t v);  

/** returns the resultant vector of an addition operation of two vectors */
vec3_t vec3_add(vec3_t v1, vec3_t v2);

/** returns the resultant vector of an subtraction operation of two vectors */
vec3_t vec3_sub(vec3_t v1, vec3_t v2);

/** returns the resultant vector of an multiply operation between a vector and a scalar factor */
vec3_t vec3_mul(vec3_t v, float factor);

/** returns the resultant vector of an division operation between a vector and a scalar factor */
vec3_t vec3_div(vec3_t v, float factor);

/** returns the resultant vector of a cross product operation between two vectors */
vec3_t vec3_cross(vec3_t v1, vec3_t v2);

/** returns the resultant scalar value of a dot product operation between two vectors */
float vec3_dot(vec3_t v1, vec3_t v2);

vec3_t vector_rotate_x(vec3_t vec, float angle);
vec3_t vector_rotate_y(vec3_t vec, float angle);
vec3_t vector_rotate_z(vec3_t vec, float angle);

#endif