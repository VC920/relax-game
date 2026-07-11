#ifndef MESH_H
#define MESH_H

typedef struct {
    float position[3];
    float normal[3];
    float texcoords[2];
} Vertex;

typedef struct {
    unsigned int vao_id;
    int index_count;
} Mesh;

Mesh mesh_create(Vertex *vertices, unsigned int *indices);

#endif//MESH_H