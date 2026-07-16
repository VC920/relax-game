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

    camera.mouse_xrel = 0;
    camera.mouse_yrel = 0;

    camera.can_fly = false;
    camera.move_back = false;
    camera.move_front = false;
    camera.move_left = false;
    camera.move_right = false;
    camera.move_up = false;
    camera.move_down = false;
    return camera;
}

void camera_update(Camera *camera, float dt)
{
    // rot
    camera->yaw += camera->mouse_xrel * camera->sensitivity;
    camera->pitch -= camera->mouse_yrel * camera->sensitivity;

    camera->mouse_xrel = 0;
    camera->mouse_yrel = 0;

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

    // can fly
    if (camera->can_fly) {
	if (camera->move_up) {
	    glm_vec3_scale(camera->up, -camera->speed * dt, move_dir);
	    glm_vec3_add(camera->position, move_dir, camera->position);
	}
	if (camera->move_down) {
	    glm_vec3_scale(camera->up, camera->speed * dt, move_dir); 
	    glm_vec3_add(camera->position, move_dir, camera->position);
	}
    } else {
	camera->position[1] = 1.0f;
    }

    glm_vec3_add(camera->position, camera->front, camera->target);
}
