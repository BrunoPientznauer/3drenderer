#include "matrix.h"
#include "camera.h"

static camera_t camera = {
    .position = {0, 0, 0},
    .direction = {0, 0, 1},
    .forward_velocity = {0, 0, 0},
    .yaw = 0.0f,
    .pitch = 0.0f
};

vec3_t get_camera_position(void)
{
    return camera.position;
}

vec3_t get_camera_direction(void)
{
    return camera.direction;
}

void update_camera_direction(vec3_t value)
{
    camera.direction = value;
}

void update_camera_position_x(float value)
{
   camera.position.x = value;
}

void update_camera_position_y(float value)
{
    camera.position.y = value;
}

void update_camera_position_z(float value)
{
    camera.position.z = value;
}

void update_camera_position(vec3_t value)
{
    camera.position = value;
}

void update_camera_yaw(float value)
{
    camera.yaw = value;
}

float get_camera_yaw(void)
{
    return camera.yaw;
}

void update_camera_pitch(float value)
{
    camera.pitch = value;
}

float get_camera_pitch(void)
{
    return camera.pitch;
}

void update_camera_forward_velocity(vec3_t value)
{
    camera.forward_velocity = value;
}

vec3_t get_camera_forward_velocity(void)
{
    return camera.forward_velocity;
}

vec3_t get_camera_lookat_target(void)
{
    vec3_t target = {0, 0, 1};

	mat4_t camera_yaw_rotation = mat4_make_rotation_y(get_camera_yaw());
	mat4_t camera_pitch_rotation = mat4_make_rotation_x(get_camera_pitch());

    mat4_t camera_rotation = mat4_identity();
	camera_rotation = mat4_mul_mat4(camera_yaw_rotation, camera_rotation);
    camera_rotation = mat4_mul_mat4(camera_pitch_rotation, camera_rotation);

	update_camera_direction(vec3_from_vec4(mat4_mul_vec4(camera_rotation, vec4_from_vec3(target))));

	target = vec3_add(get_camera_position(), get_camera_direction());

    return target;
}
