#ifndef CAMERA_H 
#define CAMERA_H

#include "vectors.h"

typedef struct{
    vec3_t position;
    vec3_t direction;
}camera_t;

extern camera_t camera;
#endif