#ifndef LIGHT_H
#define LIGHT_H

#include "vectors.h"
#include <stdint.h>

typedef struct{
	vec3_t direction;
}light_t;

vec3_t get_light_direction(void);
void init_light(vec3_t direction);

uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);

#endif