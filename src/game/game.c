#include "game.h"
#include <stdio.h>
#include <stdbool.h>
#include <stb_ds.h>
#include <math.h>
#include "entity.h"
#include "loader.h"
#include "renderer.h"

#define PLANE_INDEX 0

static bool keys[512];
Entity entities[512];

void game_init()
{
    // Model demo_model = load_model("assets/model/Untitled.obj");
    // Model demo_model = load_model("assets/model/torus/torus.obj");
    // demo_model.texture = load_texture("assets/model/torus/awesomeface.png");
    Model demo_model = load_model("assets/model/backpack/backpack.obj");
    demo_model.texture = load_texture("assets/model/backpack/diffuse.jpg");

    entities[PLANE_INDEX] = entity_default();
    entities[PLANE_INDEX].model = demo_model;
}

void game_update(float dt)
{
    // Update
    // increase_rotation(&entities[PLANE_INDEX], 0, dt, 0);
    
    if (keys['w']) {
        entities[PLANE_INDEX].position[2] += 10 * dt;
    }
    if (keys['s']) {
        entities[PLANE_INDEX].position[2] -= 10 * dt;
    }
    if (keys['a']) {
        entities[PLANE_INDEX].position[0] += 10 * dt;
    }
    if (keys['d']) {
        entities[PLANE_INDEX].position[0] -= 10 * dt;
    }
    if (keys['q']) {
        entities[PLANE_INDEX].position[1] += 10 * dt;
    }
    if (keys['e']) {
        entities[PLANE_INDEX].position[1] -= 10 * dt;
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