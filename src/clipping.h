#ifndef CLIPPING_H
#define CLIPPING_H

#include "vectors.h"
#include "triangle.h"

#define MAX_NUM_POLY_VERTICES 10
#define MAX_NUM_TRIANGLES 10

enum{
    LEFT_PLANE_FRUSTRUM,
    RIGHT_PLANE_FRUSTRUM,
    TOP_PLANE_FRUSTRUM,
    BOTTOM_PLANE_FRUSTRUM,
    NEAR_PLANE_FRUSTRUM,
    FAR_PLANE_FRUSTRUM
};

typedef struct{
    vec3_t point;
    vec3_t normal;
} plane_t;


typedef struct{
    vec3_t vertices[MAX_NUM_POLY_VERTICES];
    int num_of_vertices;
}polygon_t;


// https://courses.pikuma.com/courses/take/learn-computer-graphics-programming/lessons/15960054-defining-frustum-planes-points-normals
void initialize_frustrum_planes(float fov_x, float fov_y, float z_near, float z_far);
polygon_t create_polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2);
void clip_polygon(polygon_t* polygon);
void clip_polygon_against_plane(polygon_t* polygon, int frustrum_plane);
void create_triangle_from_polygon(polygon_t* polygon, triangle_t triangles[], int* num_triangles);

#endif