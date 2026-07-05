#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "texture.h"

typedef struct {
    Mesh mesh;
    Texture texture;
} Model;

#endif//MODEL_H