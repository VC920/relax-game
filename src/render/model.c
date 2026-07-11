#include "model.h"
#include <stdio.h>
#include <stb_ds.h>

Model model_load(const char *path)
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
    Mesh mesh = mesh_create(vertices, indices);

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