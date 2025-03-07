#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>

typedef struct{
    float x;
    float y;
} vec2_t;

typedef struct{
    float x;
    float y;
    float z;
} vec3_t;

//TODO: functions declarations

vec3_t vector_rotate_x(vec3_t vec, float angle);
vec3_t vector_rotate_y(vec3_t vec, float angle);
vec3_t vector_rotate_z(vec3_t vec, float angle);

#endif