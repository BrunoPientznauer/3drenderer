#include "matrix.h"
#include "math.h"

mat4_t mat4_identity(void)
{
    mat4_t m = {{{1, 0, 0, 0},
                 {0, 1, 0, 0},
                 {0, 0, 1, 0},
                 {0, 0, 0, 1}}};

    return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
    mat4_t scale_matrix = mat4_identity();

    scale_matrix.m[0][0] = sx;
    scale_matrix.m[1][1] = sy;
    scale_matrix.m[2][2] = sz;

    return scale_matrix;
}

mat4_t mat4_make_translation(float tx, float ty, float tz)
{
    mat4_t translation_matrix = mat4_identity();

    translation_matrix.m[0][3] = tx;
    translation_matrix.m[1][3] = ty;
    translation_matrix.m[2][3] = tz;

    return translation_matrix;
}

mat4_t mat4_make_rotation_z(float angle)
{
    // calculates the sin and cosine
    float c = cos(angle);
    float s = sin(angle);

    mat4_t rotation_matrix = mat4_identity();

    // sets the x values
    rotation_matrix.m[0][0] = c;
    rotation_matrix.m[0][1] = -s;

    // sets the y values
    rotation_matrix.m[1][0] = s;
    rotation_matrix.m[1][1] = c;

    return rotation_matrix;
}

mat4_t mat4_make_rotation_x(float angle)
{
    // calculates the sin and cosine
    float c = cos(angle);
    float s = sin(angle);

    mat4_t rotation_matrix = mat4_identity();

    // sets the y values
    rotation_matrix.m[1][1] = c;
    rotation_matrix.m[1][2] = -s;

    // sets the z values
    rotation_matrix.m[2][1] = s;
    rotation_matrix.m[2][2] = c;

    return rotation_matrix;
}

mat4_t mat4_make_rotation_y(float angle)
{
    // calculates the sin and cosine
    float c = cos(angle);
    float s = sin(angle);

    mat4_t rotation_matrix = mat4_identity();

    // sets the x values
    rotation_matrix.m[0][0] = c;
    rotation_matrix.m[0][2] = s;

    // sets the z values
    rotation_matrix.m[2][0] = -s;
    rotation_matrix.m[2][2] = c;

    return rotation_matrix;
}

mat4_t mat4_make_perspective(float aspect, float fov, float zfar, float znear)
{
    // | (h/w)*1/tan(fov/2)             0              0                 0 |
    // |                  0  1/tan(fov/2)              0                 0 |
    // |                  0             0     zf/(zf-zn)  (-zf*zn)/(zf-zn) |
    // |                  0             0              1                 0 |
    mat4_t result = {{{0}}};

    result.m[0][0] = (aspect * (1 / tan(fov / 2)));
    result.m[1][1] = 1 / tan(fov / 2);
    result.m[2][2] = zfar / (zfar - znear);
    result.m[2][3] = (-zfar * znear) / (zfar - znear);
    result.m[3][2] = 1.0;

    return result;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
    vec4_t result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    return result;
}

mat4_t mat4_mul_mat4(mat4_t m1, mat4_t m2)
{
    mat4_t result;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = (m1.m[i][0] * m2.m[0][j]) + (m1.m[i][1] * m2.m[1][j]) + (m1.m[i][2] * m2.m[2][j]) + (m1.m[i][3] * m2.m[3][j]);
        }
    }

    return result;
}

vec4_t mat4_mult_vec4_project(mat4_t projection_matrix, vec4_t v)
{
    // multiply the projection matrix by the original vector.
    vec4_t result =  mat4_mul_vec4(projection_matrix, v);

    // perform perspective divide with the original value that is now stored on w.
    if(result.w != 0){
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }

    return result;
}
