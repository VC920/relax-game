#include "game.h"
#include <stdio.h>
#include <stdbool.h>
#include <stb_ds.h>
#include <cglm/cglm.h>
#include "entity.h"
#include "renderer.h"
#include "map.h"
#include "camera.h"

typedef enum {
    NORMAL,
    EDITOR
} game_state;

static Shader entity_shader;
static Shader light_shader;
static Entity *entities;
static Entity light;
static Camera camera;
static game_state main_state; 

void game_init()
{
    // Init renderer
    entity_shader = shader_load("assets/shader/light.vert", "assets/shader/light.frag");
    light_shader = shader_load("assets/shader/light.vert", "assets/shader/sun.frag");
    renderer_init();

    // Init entities
    // entities = map_load_entities("assets/light_test.map");
    entities = map_load_entities("assets/main.map");

    Model cube_model = model_load("assets/model/cube.obj");
    light = entity_init(&cube_model);
    light.position[0] = 20;
    light.position[1] = 20;
    light.position[2] = 20;


    // Init camera
    vec3 cam_pos = { 0.0f, 1.0f, 0.0f };
    camera = camera_init(cam_pos, 5.0f, 0.05f);

    // Init mode
    main_state = NORMAL;
}

void game_update(float dt)
{
    camera_update(&camera, dt);

    // camera fly
    if (main_state != EDITOR) {
        camera.position[1] = 1.0f;
    }

    // Render
    renderer_clear();
    for (int i = 0; i < arrlen(entities); i++) {
        renderer_render(&entity_shader, &light, &entities[i], &camera);
    }
    renderer_render(&light_shader, &light, &light, &camera);
}

void game_key_down(char key)
{
    switch (key) {
    case 'w':
        camera.move_front = true;
        break;
    case 's':
        camera.move_back = true;
        break;
    case 'a':
        camera.move_left = true;
        break;
    case 'd':
        camera.move_right = true;
    default:
        break;
    }
}

void game_key_up(char key)
{
    switch (key) {
    case 'w':
        camera.move_front = false;
        break;
    case 's':
        camera.move_back = false;
        break;
    case 'a':
        camera.move_left = false;
        break;
    case 'd':
        camera.move_right = false;
    default:
        break;
    }
}

void game_key_pressed(char key)
{
    switch (key) {
    case '\t':
        if (main_state == NORMAL) {
            main_state = EDITOR;
        } else if (main_state == EDITOR) {
            main_state = NORMAL;
        }
        break;
    default:
        break;
    };
}

void game_mouse_relative_pos(int x, int y)
{
    camera.mouse_xrel = x;
    camera.mouse_yrel = y;
}
