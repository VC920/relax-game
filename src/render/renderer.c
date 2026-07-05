#include "renderer.h"
#include <stdio.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

static Shader entity_shader;
static unsigned int projection_location;
static unsigned int model_location;

void renderer_init(Shader *shader)
{
    entity_shader = *shader;

    // Setting
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glClearColor(0.5, 0.5, 0.5, 1.0);
    
    // Uniform location cache
    projection_location = glGetUniformLocation(shader->program_id, "projection_mat");
    model_location = glGetUniformLocation(shader->program_id, "model_mat");

    // Projection matrix
    mat4 projection_mat;
    glm_mat4_identity(projection_mat);
    glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 100.0f, projection_mat);

    glUseProgram(shader->program_id);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (float*)projection_mat);
}

void create_transform_mat(mat4 model_mat, float *position, float *rotation, float scale)
{
    glm_mat4_identity(model_mat);
    glm_translate(model_mat, (vec3){position[0], position[1], position[2]});
    glm_rotate(model_mat, rotation[0], (vec3){1, 0, 0});
    glm_rotate(model_mat, rotation[1], (vec3){0, 1, 0});
    glm_rotate(model_mat, rotation[2], (vec3){0, 0, 1});
    glm_scale(model_mat, (vec3){scale, scale, scale});
}

void renderer_render(Entity *entity)
{
    glUseProgram(entity_shader.program_id);

    // Model matrix
    mat4 model_mat;
    create_transform_mat(
        model_mat, 
        entity->position,
        entity->rotation, 
        entity->scale
    );
    glUniformMatrix4fv(model_location, 1, GL_FALSE, (float*)model_mat);

    Model model = entity->model;

    // Texture
    Texture texture = model.texture;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.texture_id);

    // Mesh
    Mesh mesh = model.mesh;
    glBindVertexArray(mesh.vao_id);
    glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, 0);
}
