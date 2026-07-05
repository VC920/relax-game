#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "vertex.h"
#include "mesh.h"
#include "model.h"
#include "texture.h"
#include "shader.h"

Model load_model(const char *path);

Shader load_shader(const char *vertex_path, const char *fragment_path);

Texture load_texture(const char *path);

#endif//MODEL_LOADER_H