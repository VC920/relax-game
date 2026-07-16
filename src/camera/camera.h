#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    vec3 up;
    vec3 front;
    vec3 right;
    vec3 target;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    float mouse_xrel;
    float mouse_yrel;

    bool can_fly;
    bool move_front;
    bool move_back;
    bool move_left;
    bool move_right;
    bool move_up;
    bool move_down;
} Camera;

Camera camera_init(vec3 position, float speed, float sensitivity);

void camera_update(Camera *camera, float dt);

#endif//CAMERA_H
