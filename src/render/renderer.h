#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "entity.h"
#include "camera.h"
#include "light.h"

void renderer_init();
void renderer_clear();
void renderer_render(Shader *shader, Light *light, Entity *entity, Camera *camera);

#endif//RENDERER_H
