#ifndef ENTITY_H
#define ENTITY_H

#include "model.h"

typedef struct {
    Model model;
    float position[3];
    float rotation[3];
    float scale;
} Entity;

Entity entity_default();
void increase_position(Entity *entity, float dx, float dy, float dz);
void increase_rotation(Entity *entity, float dx, float dy, float dz);

#endif//ENTITY_H