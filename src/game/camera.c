#include "camera.h"
#include <SDL2/SDL.h>

Camera camera_init(vec3 position, float speed, float sensitivity)
{
    Camera camera;
    glm_vec3_copy(position, camera.position);
    glm_vec3_copy((vec3){0.0, 1.0, 0.0}, camera.up);
    camera.speed = speed;
    camera.sensitivity = sensitivity;
    camera.pitch = 0;
    camera.yaw = 0;
    return camera;
}

void camera_update(Camera *camera, int mouse_xrel, int mouse_yrel, float dt)
{
    // rot
    camera->yaw += mouse_xrel * camera->sensitivity;
    camera->pitch -= mouse_yrel * camera->sensitivity;

    if (camera->pitch > 89.0f) {
        camera->pitch = 89.0f;
    }
    if (camera->pitch < -89.0f) {
        camera->pitch = -89.0f;
    }

    camera->front[0] = cos(glm_rad(camera->pitch)) * sin(glm_rad(camera->yaw));
    camera->front[1] = sin(glm_rad(camera->pitch));
    camera->front[2] = cos(glm_rad(camera->pitch)) * -cos(glm_rad(camera->yaw));
    glm_vec3_normalize(camera->front);
    
    glm_vec3_cross(camera->front, camera->up, camera->right);
    glm_vec3_normalize(camera->right);

    // pos
    vec3 move_dir;
    if (camera->move_front) {
        glm_vec3_scale(camera->front, camera->speed * dt, move_dir);
        glm_vec3_add(camera->position, move_dir, camera->position);
    }
    if (camera->move_back) {
        glm_vec3_scale(camera->front, -camera->speed * dt, move_dir);
        glm_vec3_add(camera->position, move_dir, camera->position);
    }
    if (camera->move_left) {
        glm_vec3_scale(camera->right, -camera->speed * dt, move_dir);
        glm_vec3_add(camera->position, move_dir, camera->position);
    }
    if (camera->move_right) {
        glm_vec3_scale(camera->right, camera->speed * dt, move_dir); 
        glm_vec3_add(camera->position, move_dir, camera->position);
    }

    camera->position[1] = 1;

    glm_vec3_add(camera->position, camera->front, camera->target);
}
