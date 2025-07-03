#include <math.h>
#include "clipping.h"

#define NUM_OF_PLANES 6

plane_t frustrum_planes[NUM_OF_PLANES];

void initialize_frustrum_planes(float fov, float z_near, float z_far)
{
    float cos_half_fov = cos(fov / 2);
    float sin_half_fov = sin(fov / 2);

    frustrum_planes[LEFT_PLANE_FRUSTRUM].point = vec3_new(0, 0, 0);
    frustrum_planes[LEFT_PLANE_FRUSTRUM].normal = vec3_new(cos_half_fov, 0, sin_half_fov);

    frustrum_planes[RIGHT_PLANE_FRUSTRUM].point = vec3_new(0, 0, 0);
    frustrum_planes[RIGHT_PLANE_FRUSTRUM].normal = vec3_new(-cos_half_fov, 0, sin_half_fov);

    frustrum_planes[TOP_PLANE_FRUSTRUM].point = vec3_new(0, 0, 0);
    frustrum_planes[TOP_PLANE_FRUSTRUM].normal = vec3_new(0, -cos_half_fov, sin_half_fov);

    frustrum_planes[BOTTOM_PLANE_FRUSTRUM].point = vec3_new(0, 0, 0);
    frustrum_planes[BOTTOM_PLANE_FRUSTRUM].normal = vec3_new(0, cos_half_fov, sin_half_fov);

    frustrum_planes[NEAR_PLANE_FRUSTRUM].point = vec3_new(0, 0, z_near);
    frustrum_planes[NEAR_PLANE_FRUSTRUM].normal = vec3_new(0, 0, 1);

    frustrum_planes[FAR_PLANE_FRUSTRUM].point = vec3_new(0, 0, z_far);
    frustrum_planes[FAR_PLANE_FRUSTRUM].normal = vec3_new(0, 0, -1);
}

polygon_t create_polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2)
{
    polygon_t result = {
        .vertices = {v0, v1, v2},
        .num_of_vertices = 3
    };

    return result;
}

void clip_polygon(polygon_t *polygon)
{
    clip_polygon_against_plane(polygon, LEFT_PLANE_FRUSTRUM);
    clip_polygon_against_plane(polygon, RIGHT_PLANE_FRUSTRUM);
    clip_polygon_against_plane(polygon, BOTTOM_PLANE_FRUSTRUM);
    clip_polygon_against_plane(polygon, TOP_PLANE_FRUSTRUM);
    clip_polygon_against_plane(polygon, NEAR_PLANE_FRUSTRUM);
    clip_polygon_against_plane(polygon, FAR_PLANE_FRUSTRUM);
}

void clip_polygon_against_plane(polygon_t *polygon, int frustrum_plane)
{
}
