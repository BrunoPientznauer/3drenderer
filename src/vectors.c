#include <math.h>
#include "vectors.h"

////////////////////////////////////////////////////////////////////////////////////////////
// Vector 2 Functions
////////////////////////////////////////////////////////////////////////////////////////////

vec2_t vec2_new(float x, float y)
{
    vec2_t result = {x, y};
    return result;
}

float vec2_length(vec2_t v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

vec2_t vec2_add(vec2_t a, vec2_t b)
{
    vec2_t result = {
        .x = a.x + b.x,
        .y = a.y + a.y
    };

    return result;
}

vec2_t vec2_sub(vec2_t a, vec2_t b)
{
    vec2_t result = {
        .x = a.x - b.x,
        .y = a.y - b.y
    };

    return result;
}

vec2_t vec2_mul(vec2_t v, float factor)
{
    vec2_t result = {
        .x = v.x * factor,
        .y = v.y * factor
    };

    return result;
}

vec2_t vec2_div(vec2_t v, float factor)
{
    vec2_t result = {
        .x = v.x / factor,
        .y = v.y / factor
    };

    return result;}

float vec2_dot(vec3_t a, vec3_t b)
{
    return (a.x * b.x) + (a.y * b.y);
}


void vec2_normalize(vec2_t* v)
{
    float magnitude = vec2_length(*v);
    v->x /= magnitude;
    v->y /= magnitude;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Vector 3 Functions
////////////////////////////////////////////////////////////////////////////////////////////

vec3_t vec3_new(float x, float y, float z)
{
    vec3_t result = {x, y, z};
    return result;
}

float vec3_length(vec3_t v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vec3_t vec3_add(vec3_t a, vec3_t b)
{
    vec3_t result = {
        .x = a.x + b.x,
        .y = a.y + a.y,
        .z = a.z + b.z
    };

    return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b)
{
    vec3_t result = {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };

    return result;
}

vec3_t vec3_mul(vec3_t v, float factor)
{
    vec3_t result = {
        .x = v.x * factor,
        .y = v.y * factor,
        .z = v.z * factor
    };

    return result;
}

vec3_t vec3_div(vec3_t v, float factor)
{
    vec3_t result = {
        .x = v.x / factor,
        .y = v.y / factor,
        .z = v.z / factor
    };

    return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    vec3_t result = {
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x
    };
    return result;
}

float vec3_dot(vec3_t a, vec3_t b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void vec3_normalize(vec3_t* v)
{
    float magnitude = vec3_length(*v);
    v->x /= magnitude;
    v->y /= magnitude;
    v->z /= magnitude;
}

vec3_t vector_rotate_x(vec3_t vec, float angle)
{
    vec3_t rotated_vector = {
        .x = vec.x,
        .y = vec.y * cos(angle) - vec.z * sin(angle),
        .z = vec.y * sin(angle) + vec.z * cos(angle)};

    return rotated_vector;
}

vec3_t vector_rotate_y(vec3_t vec, float angle)
{
    vec3_t rotated_vector = {
        .x = vec.x * cos(angle) - vec.z * sin(angle),
        .y = vec.y,
        .z = vec.x * sin(angle) + vec.z * cos(angle)};

    return rotated_vector;
}

vec3_t vector_rotate_z(vec3_t vec, float angle)
{
    vec3_t rotated_vector = {
        .x = vec.x * cos(angle) - vec.y * sin(angle),
        .y = vec.x * sin(angle) + vec.y * cos(angle),
        .z = vec.z};

    return rotated_vector;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Vector Convertion Functions
////////////////////////////////////////////////////////////////////////////////////////////

vec4_t vec4_from_vec3(vec3_t vec3)
{
    vec4_t v = {.x = vec3.x, .y = vec3.y, .z = vec3.z, .w = 1.0};
    return v;
}

vec3_t vec3_from_vec4(vec4_t vec4)
{
    vec3_t v = {.x = vec4.x, .y = vec4.y, .z = vec4.z};
    return v;
}


vec2_t vec2_from_vec4(vec4_t v)
{
    vec2_t result = {.x = v.x, .y =  v.y};
    return result;
}