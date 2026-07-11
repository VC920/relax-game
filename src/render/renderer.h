#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "entity.h"
#include "camera.h"

void renderer_init(Shader *shader);
void renderer_clear();
void renderer_render(Entity *entity, Camera *camera);

#endif//RENDERER_H