#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"
#include "entity.h"

void renderer_init(Shader *shader);
void renderer_render(Entity *entity);

#endif//RENDERER_H