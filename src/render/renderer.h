#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "entity.h"
#include "camera.h"

void renderer_init();
void renderer_clear();
void renderer_render(Shader *shader, Entity *light, Entity *entity, Camera *camera);

#endif//RENDERER_H
