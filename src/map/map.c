#include "map.h"
#include <stdio.h>
#include <stb_ds.h>

Entity *map_load_entities(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error not found %s\n", path);
        return NULL;
    }

    // Read file
    Entity *entities = NULL;

    char line[256]; // one line size
    while (fgets(line, sizeof(line), file) != NULL) {
        Entity entity;
        char entity_name[256] = {0};
        char model_path[256] = {0};
        char texture_path[256] = {0};
        sscanf(line, "%s%s%s [%f/%f/%f] [%f/%f/%f] [%f]",
            entity_name, model_path, texture_path,
            &entity.position[0], &entity.position[1], &entity.position[2],
            &entity.rotation[0], &entity.rotation[1], &entity.rotation[2],
            &entity.scale
        );

        Model model = model_load(model_path);
        if (strcmp(texture_path, "NULL") != 0) {
            model.texture = texture_load(texture_path);
        }

        entity.model = model;
        arrpush(entities, entity);

        // Log
        // printf("\n\nentity name: %s\n", entity_name);
        // printf("entity model path: %s\n", model_path);
        // printf("entity texture path: %s\n", texture_path);
        // printf("entity position: %f, %f, %f\n", entity.position[0], entity.position[1], entity.position[2]);
        // printf("entity rotation: %f, %f, %f\n", entity.rotation[0], entity.rotation[1], entity.rotation[2]);
    }

    fclose(file);
    return entities;
}
