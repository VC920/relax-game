#include "entity.h"

Entity entity_default()
{
    return (Entity) { 
        .position = { 0, 0, 0 },
        .rotation = { 0, 0, 0 },
        .scale = 1
    };
}

void increase_position(Entity *entity, float dx, float dy, float dz)
{
    entity->position[0] += dx;
    entity->position[1] += dy;
    entity->position[2] += dz;
}

void increase_rotation(Entity *entity, float dx, float dy, float dz)
{
    entity->rotation[0] += dx;
    entity->rotation[1] += dy;
    entity->rotation[2] += dz;
}