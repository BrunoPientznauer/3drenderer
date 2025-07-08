#ifndef CAMERA_H 
#define CAMERA_H

#include "vectors.h"

typedef struct{
    vec3_t position;
    vec3_t direction;
    vec3_t forward_velocity;
    float yaw;
    float pitch;
}camera_t;

vec3_t get_camera_position(void);

vec3_t get_camera_direction(void);
void update_camera_direction(vec3_t value);

void update_camera_position_x(float value);
void update_camera_position_y(float value);
void update_camera_position_z(float value);
void update_camera_position(vec3_t value);

void update_camera_yaw(float value);
float get_camera_yaw(void);

void update_camera_pitch(float value);
float get_camera_pitch(void);

void update_camera_forward_velocity(vec3_t value);
vec3_t get_camera_forward_velocity(void);

vec3_t get_camera_lookat_target(void);

#endif