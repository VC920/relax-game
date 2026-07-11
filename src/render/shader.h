#ifndef SHADER_H
#define SHADER_H

typedef struct {
    unsigned int program_id;
} Shader;

Shader shader_load(const char *vertex_path, const char *fragment_path);

#endif//SHADER_H