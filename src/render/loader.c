#include "loader.h"
#include <stdio.h>
#include <glad/glad.h>

#include <stb_ds.h>

/*
 *  Tool
 */

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

/*
 *  Load model
 */

unsigned int create_vao()
{
    unsigned int vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    return vao_id;
}

void bind_indices(unsigned int *indices) {
    unsigned int ebo_id;
    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlen(indices) * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

void bind_vertices(Vertex *vertices)
{
    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, arrlen(vertices) * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));
}

Mesh create_mesh(Vertex *vertices, unsigned int *indices)
{
    unsigned int vao_id = create_vao();
    bind_indices(indices);
    bind_vertices(vertices);

    return (Mesh) {
        .vao_id = vao_id,
        .index_count = arrlen(indices)
    };
}

Model load_model(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error not found %s\n", path);
        return (Model){};
    }

    // Read file
    int index_count = 0;
    float *positions = NULL;
    float *normals = NULL;
    float *texcoords = NULL;
    int *pos_indices = NULL;
    int *nor_indices = NULL;
    int *tex_indices = NULL;

    char line[256]; // one line size
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == 'v' && line[1] == ' ') {
            float p[3];
            sscanf(line, "v %f %f %f", &p[0], &p[1], &p[2]);
            arrpush(positions, p[0]);
            arrpush(positions, p[1]);
            arrpush(positions, p[2]);
        } else if (line[0] == 'v' && line[1] == 'n') {
            float n[3];
            sscanf(line, "vn %f %f %f", &n[0], &n[1], &n[2]);
            arrpush(normals, n[0]);
            arrpush(normals, n[1]);
            arrpush(normals, n[2]);
        } else if (line[0] == 'v' && line[1] == 't') {
            float t[2];
            sscanf(line, "vt %f %f", &t[0], &t[1]);
            arrpush(texcoords, t[0]);
            arrpush(texcoords, t[1]);
        } else if (line[0] == 'f') {
            int pos_index[3];
            int nor_index[3];
            int tex_index[3];
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &pos_index[0], &tex_index[0], &nor_index[0],
                &pos_index[1], &tex_index[1], &nor_index[1],
                &pos_index[2], &tex_index[2], &nor_index[2]
            );
            for (int i = 0; i < 3; i++) {
                arrpush(pos_indices, pos_index[i]);
                arrpush(nor_indices, nor_index[i]);
                arrpush(tex_indices, tex_index[i]);
            }
            index_count += 3;
        }
    }

    fclose(file);

    // Vertices & Indices
    Vertex *vertices = NULL;
    unsigned int *indices = NULL;
    arrsetlen(indices, index_count);

    for (int i = 0; i < index_count; i++) {
        indices[i] = i;
        Vertex vertex;
        vertex.position[0] = positions[(pos_indices[i] - 1) * 3];
        vertex.position[1] = positions[(pos_indices[i] - 1) * 3 + 1];
        vertex.position[2] = positions[(pos_indices[i] - 1) * 3 + 2];
        vertex.normal[0] = normals[(nor_indices[i] - 1) * 3];
        vertex.normal[1] = normals[(nor_indices[i] - 1) * 3 + 1];
        vertex.normal[2] = normals[(nor_indices[i] - 1) * 3 + 2];
        vertex.texcoords[0] = texcoords[(tex_indices[i] - 1) * 2];
        vertex.texcoords[1] = texcoords[(tex_indices[i] - 1) * 2 + 1];
        arrpush(vertices, vertex);
    }

    // Mesh & Texture
    Mesh mesh = create_mesh(vertices, indices);

    arrfree(positions);
    arrfree(normals);
    arrfree(texcoords);
    arrfree(pos_indices);
    arrfree(nor_indices);
    arrfree(tex_indices);
    arrfree(vertices);
    arrfree(indices);

    return (Model) {
        .mesh = mesh
    };
}

/*
 *  Load shader
 */

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

Shader load_shader(const char *vertex_path, const char *fragment_path)
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

/*
 *  Load Texture
 */

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
Texture load_texture(const char *path)
{
    int width, height, color_bit;
    unsigned char *data = stbi_load(path, &width, &height, &color_bit, 0);
    if (data == NULL) {
        fprintf(stderr, "stbi_load Error: %s\n", path);
    }

    GLenum format;
    if (color_bit == 1) format = GL_RED;
    else if (color_bit == 3) format = GL_RGB;
    else if (color_bit == 4) format = GL_RGBA; 

    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return (Texture) {
        .texture_id = texture
    };
}
