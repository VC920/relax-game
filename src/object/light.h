#ifndef LIGHT_H
#define LIGHT_H

#include "model.h"
#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} Light;

Light light_init(float x, float y, float z);
void light_set_ambient(Light *light, float r, float g, float b);
void light_set_diffuse(Light *light, float r, float g, float b);
void light_set_specular(Light *light, float r, float g, float b);

#endif//LIGHT_H
