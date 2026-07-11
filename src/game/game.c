#include "game.h"
#include <stdio.h>
#include <stdbool.h>
#include <stb_ds.h>
#include <cglm/cglm.h>
#include "entity.h"
#include "renderer.h"
#include "map.h"
#include "camera.h"

static bool keys[512];
static int mouse_xrel;
static int mouse_yrel;
static Entity *entities;
static Camera camera;

void game_init()
{
    // Init renderer
    Shader shader = shader_load("assets/shader/shader.vert", "assets/shader/shader.frag");
    renderer_init(&shader);

    // Init entities
    entities = map_load_entities("assets/main.map");

    // Init camera
    vec3 cam_pos = { 0.0f, 1.0f, 3.0f };
    camera = camera_init(cam_pos, 5.0f, 0.05f);
}

void game_update(float dt)
{   
    camera.move_front = keys['w'];
    camera.move_back = keys['s'];
    camera.move_left = keys['a'];
    camera.move_right = keys['d'];
    camera_update(&camera, mouse_xrel, mouse_yrel, dt);

    mouse_xrel = 0;
    mouse_yrel = 0;

    // Render
    renderer_clear();
    for (int i = 0; i < arrlen(entities); i++) {
        renderer_render(&entities[i], &camera);
    }
}

void game_key_down(char key)
{
    keys[(int)key] = true; 
}

void game_key_up(char key)
{
    keys[(int)key] = false;
}

void game_mouse_relative_pos(int x, int y)
{
    mouse_xrel = x;
    mouse_yrel = y;
}