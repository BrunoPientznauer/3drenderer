#include "vectors.h"

//TODO: functions implementations

vec3_t vector_rotate_x(vec3_t vec, float angle){
    vec3_t rotated_vector = {
        .x = vec.x,
        .y = vec.y * cos(angle) - vec.z * sin(angle),
        .z = vec.y * sin(angle) + vec.z * cos(angle)
    };

    return rotated_vector;
}

vec3_t vector_rotate_y(vec3_t vec, float angle){
    vec3_t rotated_vector = {
        .x = vec.x * cos(angle) - vec.z * sin(angle),
        .y = vec.y,
        .z = vec.x * sin(angle) + vec.z * cos(angle)  
    };

    return rotated_vector;
}

vec3_t vector_rotate_z(vec3_t vec, float angle){
    vec3_t rotated_vector = {
        .x = vec.x * cos(angle) - vec.y * sin(angle),
        .y = vec.x * sin(angle) + vec.y * cos(angle),
        .z = vec.z  
    };

    return rotated_vector;
}