#ifndef CLIPPING_H
#define CLIPPING_H

#include "vectors.h"

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

#endif