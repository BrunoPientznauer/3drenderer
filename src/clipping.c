#include <math.h>
#include "clipping.h"

#define NUM_OF_PLANES 6

plane_t frustum_planes[NUM_OF_PLANES];

void initialize_frustrum_planes(float fov, float z_near, float z_far)
{
    float cos_half_fov = cos(fov / 2);
	float sin_half_fov = sin(fov / 2);

	frustum_planes[LEFT_PLANE_FRUSTRUM].point = vec3_new(0, 0, 0);
	frustum_planes[LEFT_PLANE_FRUSTRUM].normal.x = cos_half_fov;
	frustum_planes[LEFT_PLANE_FRUSTRUM].normal.y = 0;
	frustum_planes[LEFT_PLANE_FRUSTRUM].normal.z = sin_half_fov;

	frustum_planes[RIGHT_PLANE_FRUSTRUM].point = vec3_new(0, 0, 0);
	frustum_planes[RIGHT_PLANE_FRUSTRUM].normal.x = -cos_half_fov;
	frustum_planes[RIGHT_PLANE_FRUSTRUM].normal.y = 0;
	frustum_planes[RIGHT_PLANE_FRUSTRUM].normal.z = sin_half_fov;

	frustum_planes[TOP_PLANE_FRUSTRUM].point = vec3_new(0, 0, 0);
	frustum_planes[TOP_PLANE_FRUSTRUM].normal.x = 0;
	frustum_planes[TOP_PLANE_FRUSTRUM].normal.y = -cos_half_fov;
	frustum_planes[TOP_PLANE_FRUSTRUM].normal.z = sin_half_fov;

	frustum_planes[BOTTOM_PLANE_FRUSTRUM].point = vec3_new(0, 0, 0);
	frustum_planes[BOTTOM_PLANE_FRUSTRUM].normal.x = 0;
	frustum_planes[BOTTOM_PLANE_FRUSTRUM].normal.y = cos_half_fov;
	frustum_planes[BOTTOM_PLANE_FRUSTRUM].normal.z = sin_half_fov;

	frustum_planes[NEAR_PLANE_FRUSTRUM].point = vec3_new(0, 0, z_near);
	frustum_planes[NEAR_PLANE_FRUSTRUM].normal.x = 0;
	frustum_planes[NEAR_PLANE_FRUSTRUM].normal.y = 0;
	frustum_planes[NEAR_PLANE_FRUSTRUM].normal.z = 1;

	frustum_planes[FAR_PLANE_FRUSTRUM].point = vec3_new(0, 0, z_far);
	frustum_planes[FAR_PLANE_FRUSTRUM].normal.x = 0;
	frustum_planes[FAR_PLANE_FRUSTRUM].normal.y = 0;
	frustum_planes[FAR_PLANE_FRUSTRUM].normal.z = -1;
}

polygon_t create_polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2)
{
     polygon_t polygon = {
        .vertices = { v0, v1, v2 },
        .num_of_vertices = 3
    };
    return polygon;
}

void clip_polygon(polygon_t *polygon)
{
    clip_polygon_against_plane(polygon, LEFT_PLANE_FRUSTRUM);
    clip_polygon_against_plane(polygon, RIGHT_PLANE_FRUSTRUM);
    clip_polygon_against_plane(polygon, TOP_PLANE_FRUSTRUM);
    clip_polygon_against_plane(polygon, BOTTOM_PLANE_FRUSTRUM);
    clip_polygon_against_plane(polygon, NEAR_PLANE_FRUSTRUM);
    clip_polygon_against_plane(polygon, FAR_PLANE_FRUSTRUM);
}

void clip_polygon_against_plane(polygon_t *polygon, int frustrum_plane)
{
    vec3_t plane_point = frustum_planes[frustrum_plane].point;
    vec3_t plane_normal = frustum_planes[frustrum_plane].normal;

    // initializes the array of vertices inside the frustrum plane.
    vec3_t inside_vertices[MAX_NUM_POLY_VERTICES];
    int num_inside_vertices = 0;

    // initializes the current and previous vertices with the first and last vertices on the polygon.
    vec3_t* current_vertex = &polygon->vertices[0];
    vec3_t* previous_vertex = &polygon->vertices[polygon->num_of_vertices - 1];

    float current_dot = 0;
    float previous_dot = vec3_dot(vec3_sub(*previous_vertex, plane_point), plane_normal);

    while (current_vertex != &polygon->vertices[polygon->num_of_vertices])
    {
        current_dot = vec3_dot(vec3_sub(*current_vertex, plane_point), plane_normal);

        if(current_dot * previous_dot < 0){
            // finds the interpolation factor t
            float factor = previous_dot / (previous_dot - current_dot);

            // finds the intersection point I = Q1 + T(Q2 - Q1)
            vec3_t interscetion_point = vec3_clone(current_vertex);              // I =        Qc
            interscetion_point = vec3_sub(interscetion_point, *previous_vertex); // I =       (Qc - Qp)
            interscetion_point = vec3_mul(interscetion_point, factor);           // I =      T(Qc - Qp)
            interscetion_point = vec3_add(interscetion_point, *previous_vertex ); // I = Qp + T(Qc - Qp)

            inside_vertices[num_inside_vertices] = vec3_clone(&interscetion_point);
            num_inside_vertices++;
        }

        if(current_dot > 0){
            inside_vertices[num_inside_vertices] = vec3_clone(current_vertex);
            num_inside_vertices++;
        }
        
        previous_dot = current_dot;
        previous_vertex = current_vertex;
        current_vertex++;
    }
    
    for(int i = 0; i < num_inside_vertices; i++){
        polygon->vertices[i] = vec3_clone(&inside_vertices[i]);
    }

    polygon->num_of_vertices = num_inside_vertices;
}

void create_triangle_from_polygon(polygon_t *polygon, triangle_t triangles[], int *num_triangles)
{
    for(int i = 0; i < polygon->num_of_vertices - 2; i++){
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;

        triangles[i].points[0] = vec4_from_vec3(polygon->vertices[index0]);
        triangles[i].points[1] = vec4_from_vec3(polygon->vertices[index1]);
        triangles[i].points[2] = vec4_from_vec3(polygon->vertices[index2]);
    }

    *num_triangles = polygon->num_of_vertices - 2;
}
