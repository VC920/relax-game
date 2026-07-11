#include "renderer.h"
#include <stdio.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

static Shader entity_shader;
static unsigned int projection_location;
static unsigned int model_location;
static unsigned int view_location;

void renderer_init(Shader *shader)
{
    entity_shader = *shader;

    // Setting
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glClearColor(0.9, 0.9, 1.0, 1.0);
    
    glUseProgram(shader->program_id);

    // Uniform location cache
    projection_location = glGetUniformLocation(shader->program_id, "projection_mat");
    model_location = glGetUniformLocation(shader->program_id, "model_mat");
    view_location = glGetUniformLocation(shader->program_id, "view_mat");
}

void renderer_clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void create_transform_mat(mat4 model_mat, float *position, float *rotation, float scale)
{
    glm_mat4_identity(model_mat);
    glm_translate(model_mat, (vec3){position[0], position[1], position[2]});
    glm_rotate(model_mat, glm_rad(rotation[0]), (vec3){1, 0, 0});
    glm_rotate(model_mat, glm_rad(rotation[1]), (vec3){0, 1, 0});
    glm_rotate(model_mat, glm_rad(rotation[2]), (vec3){0, 0, 1});
    glm_scale(model_mat, (vec3){scale, scale, scale});
}

void create_lookat_mat(mat4 lookat_mat, vec3 position, vec3 target, vec3 up)
{
    glm_mat4_identity(lookat_mat);
    glm_lookat(position, target, up, lookat_mat);
}

void create_projection_mat(mat4 projection_mat, int window_width, int window_height)
{   
    glm_mat4_identity(projection_mat);
    glm_perspective(glm_rad(45.0f), (float)window_width / (float)window_height, 0.1f, 100.0f, projection_mat);
}

void renderer_render(Entity *entity, Camera *camera)
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

    // LookAt Matrix
    mat4 view_mat;
    create_lookat_mat(
        view_mat,
        camera->position,
        camera->target,
        camera->up
    );
    glUniformMatrix4fv(view_location, 1, GL_FALSE, (float*)view_mat);

    // Projection Matrix
    mat4 projection_mat;
    create_projection_mat(projection_mat, WINDOW_WIDTH, WINDOW_HEIGHT);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (float*)projection_mat);

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
