#include "light.h"

Light light_init(float x, float y, float z)
{
    return (Light) {
	.position = { x, y, z }
    };
}

void light_set_ambient(Light *light, float r, float g, float b)
{
    light->ambient[0] = r;
    light->ambient[1] = g;
    light->ambient[2] = b;
}

void light_set_diffuse(Light *light, float r, float g, float b)
{
    light->diffuse[0] = r;
    light->diffuse[1] = g;
    light->diffuse[2] = b;
}

void light_set_specular(Light *light, float r, float g, float b)
{
    light->specular[0] = r;
    light->specular[1] = g;
    light->specular[2] = b;
}
