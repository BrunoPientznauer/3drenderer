#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vectors.h"
#include <stdint.h>
#include "texture.h"
#include "upng.h"

typedef struct
{
    int a, b, c;
    tex2_t a_uv, b_uv, c_uv;
    uint32_t color;
} face_t;

typedef struct{
    vec4_t points[3];
    tex2_t texcoords[3];
    uint32_t color;
    float light_percentage;
    upng_t* texture;
}triangle_t;

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void draw_filled_triangle(int x0, int y0, float z0, float w0, int x1, int y1, float z1, float w1, int x2, int y2, float z2, float w2,  uint32_t color);

void draw_triangle_pixel(int x, int y, uint32_t color, vec4_t point_a, vec4_t point_b, vec4_t point_c);

void draw_triangle_wire(vec4_t v1, vec4_t v2, vec4_t v3, uint32_t color);

void draw_texel(int x, int y, upng_t *texture, float light_percentage,
    vec4_t point_a, vec4_t point_b, vec4_t point_c, 
    tex2_t a_uv, tex2_t b_uv, tex2_t c_uv);

void draw_textured_triangle(
    int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    upng_t* texture, float light_percentage
);

/** returns the barycentric weights alpha, beta and gamma for point p */
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);

void draw_wire(int x0, int y0, float z0, float w0, int x1, int y1, float z1, float w1, int x2, int y2, float z2, float w2,  uint32_t color);

vec3_t calculate_triangle_normal(vec4_t vertices[3]);

#endif