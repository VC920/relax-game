#include "shader.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

char *load_file_to_string(const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        fprintf(stderr, "Error not found %s\n", path);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    char *buffer = malloc(size + 1);
    fseek(f, 0, SEEK_SET);

    fread(buffer, 1, size, f);
    buffer[size] = '\0';

    fclose(f);
    return buffer;
}

void check_compile(unsigned int shader, GLenum type)
{
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        if (type == GL_VERTEX_SHADER) {
            fprintf(stderr, "glCompileShader(vertex) Error: %s\n", info_log);
        } else if (type == GL_FRAGMENT_SHADER) {
            fprintf(stderr, "glCompileShader(fragment) Error: %s\n", info_log);
        }
    };
}

void check_link(unsigned int program)
{
    int success;
    char info_log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        fprintf(stderr, "glLinkProgram Error: %s\n", info_log);
    }
}

Shader shader_load(const char *vertex_path, const char *fragment_path)
{
    char *vertex_source = load_file_to_string(vertex_path);
    char *fragment_source = load_file_to_string(fragment_path);

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_ptr = vertex_source;
    glShaderSource(vertex, 1, &vertex_ptr, NULL);
    glCompileShader(vertex);
    check_compile(vertex, GL_VERTEX_SHADER);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_ptr = fragment_source;
    glShaderSource(fragment, 1, &fragment_ptr, NULL);
    glCompileShader(fragment);
    check_compile(fragment, GL_FRAGMENT_SHADER);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    check_link(program);

    free(vertex_source);
    free(fragment_source);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return (Shader) {
        .program_id = program
    };
}