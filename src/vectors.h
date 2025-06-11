#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>

typedef struct{
    float x, y;
} vec2_t;

typedef struct{
    float x, y, z;
} vec3_t;

typedef struct{
    float x , y, z, w;
}vec4_t;


////////////////////////////////////////////////////////////////////////////////////////////
// Vector 2 Functions
////////////////////////////////////////////////////////////////////////////////////////////

/** returns the magnitude of a 2 dimensional vector */
float vec2_length(vec2_t v);

/** returns the resultant vector of an addition operation of two vectors */
vec2_t vec2_add(vec2_t a, vec2_t b);

/** returns the resultant vector of an subtraction operation of two vectors */
vec2_t vec2_sub(vec2_t a, vec2_t b);

/** returns the resultant vector of an multiply operation between a vector and a scalar factor */
vec2_t vec2_mul(vec2_t v, float factor);

/** returns the resultant vector of an division operation between a vector and a scalar factor */
vec2_t vec2_div(vec2_t v, float factor);

/** returns the resultant scalar value of a dot product operation between two vectors */
float vec2_dot(vec3_t a, vec3_t b);

/** normalize the vector passed as parameter so it has a magnitude of 1 */
void vec2_normalize(vec2_t* v);

////////////////////////////////////////////////////////////////////////////////////////////
// Vector 3 Functions
////////////////////////////////////////////////////////////////////////////////////////////

/**  returns the maginute of a 3 dimensional vector */
float vec3_length(vec3_t v);  

/** returns the resultant vector of an addition operation of two vectors */
vec3_t vec3_add(vec3_t a, vec3_t b);

/** returns the resultant vector of an subtraction operation of two vectors */
vec3_t vec3_sub(vec3_t a, vec3_t b);

/** returns the resultant vector of an multiply operation between a vector and a scalar factor */
vec3_t vec3_mul(vec3_t v, float factor);

/** returns the resultant vector of an division operation between a vector and a scalar factor */
vec3_t vec3_div(vec3_t v, float factor);

/** returns the resultant vector of a cross product operation between two vectors */
vec3_t vec3_cross(vec3_t a, vec3_t b);

/** returns the resultant scalar value of a dot product operation between two vectors */
float vec3_dot(vec3_t a, vec3_t b);

/** normalize the vector passed as parameter so it has a magnitude of 1 */
void vec3_normalize(vec3_t* v);

vec3_t vector_rotate_x(vec3_t vec, float angle);
vec3_t vector_rotate_y(vec3_t vec, float angle);
vec3_t vector_rotate_z(vec3_t vec, float angle);

////////////////////////////////////////////////////////////////////////////////////////////
// Vector convertion functions
////////////////////////////////////////////////////////////////////////////////////////////

/** creates a vec4 based on a vec3 */
vec4_t vec4_from_vec3(vec3_t vec3);

/** creates a vec3 based on a vec4 */
vec3_t vec3_from_vec4(vec4_t vec4);

/** creates a vec2 based on a vec4 */
vec2_t vec2_from_vec4(vec4_t v);

#endif