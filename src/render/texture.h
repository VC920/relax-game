#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct {
    unsigned int texture_id;
} Texture;

Texture texture_load(const char *path);

#endif//TEXTURE_H