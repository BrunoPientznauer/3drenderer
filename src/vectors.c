#include <math.h>
#include "vectors.h"

/* Vec 2D functions */

float vec2_length(vec2_t v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

vec2_t vec2_add(vec2_t v1, vec2_t v2)
{
    vec2_t result = {
        .x = v1.x + v2.x,
        .y = v1.y + v1.y
    };

    return result;
}

vec2_t vec2_sub(vec2_t v1, vec2_t v2)
{
    vec2_t result = {
        .x = v1.x - v2.x,
        .y = v1.y - v1.y
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

/* Vec 3D functions */

float vec3_length(vec3_t v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vec3_t vec3_add(vec3_t v1, vec3_t v2)
{
    vec3_t result = {
        .x = v1.x + v2.x,
        .y = v1.y + v1.y,
        .z = v1.z + v2.z
    };

    return result;
}

vec3_t vec3_sub(vec3_t v1, vec3_t v2)
{
    vec3_t result = {
        .x = v1.x - v2.x,
        .y = v1.y - v1.y,
        .z = v1.z - v2.z
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