#include <stdio.h>
#include <stdbool.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "loader.h"
#include "game.h"
#include "renderer.h"

// Settings
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

extern Entity entities[512];

int main()
{
    // Initialize window
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        fprintf(stderr, "SDL_GL_GetProcAddress Error");
    }

    // Setup
    game_init();

    Shader shader = load_shader("assets/shader/shader.vert", "assets/shader/shader.frag");
    renderer_init(&shader);

    Uint64 last_frame_time = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();
    bool running = true;

    // Main loop
    while (running)
    {
        // Process Input
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: 
                running = false;
                break;
            case SDL_KEYDOWN: 
                game_key_down(event.key.keysym.sym);
                break;
            case SDL_KEYUP: 
                game_key_up(event.key.keysym.sym);
                break;
            default:
                break;
            }
	    }

    	// Update
        Uint64 current_frame_time = SDL_GetPerformanceCounter();
        float delta_time = (float)(current_frame_time - last_frame_time) / frequency;
        last_frame_time = current_frame_time;
        // printf("Current FPS: %.f\n", 1 / delta_time);

        game_update(delta_time);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int i = 0; i < (int)(sizeof(entities) / sizeof(*entities)); i++) {
            renderer_render(&entities[i]);
        }

        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();
    return 0;
}