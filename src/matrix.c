#include "matrix.h"

mat4_t mat4_identity(void)
{
    mat4_t m = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }};

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

vec4_t mat4_mul_vec4(mat4_t mat, vec4_t vec)
{
    vec4_t result;

    result.x  = (mat.m[0][0] * vec.x) + (mat.m[0][1] * vec.y) + (mat.m[0][2] * vec.z) + (mat.m[0][3] * vec.w);
    result.y  = (mat.m[1][0] * vec.x) + (mat.m[1][1] * vec.y) + (mat.m[1][2] * vec.z) + (mat.m[1][3] * vec.w);
    result.z  = (mat.m[2][0] * vec.x) + (mat.m[2][1] * vec.y) + (mat.m[2][2] * vec.z) + (mat.m[2][3] * vec.w);
    result.w  = (mat.m[3][0] * vec.x) + (mat.m[3][1] * vec.y) + (mat.m[3][2] * vec.z) + (mat.m[3][3] * vec.w);

    return result;
}
