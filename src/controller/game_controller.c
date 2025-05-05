#include "SDL.h"
#include <stdio.h>
#include "scene_manager.h" // For scene management
#include "../model/game.h"   // For game state and logic
#include "texture_manager.h" // For texture management
#include "game_controller.h"
#include "nfd.h"

void gameInit() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    window = SDL_CreateWindow("Yukon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Initialize the Scene Manager
    sceneManager_init(window, renderer);

    // Initialize texture manager
    textureManager_init();

    // Initialize game state
    initGame();

    // Start with the startup scene
    sceneManager_changeScene(SCENE_STARTUP_MODE, 0);
}

void gameQuit() {
    // Clean up textures
    textureManager_cleanup();

    // Clean up the scene manager resources
    sceneManager_cleanup();
}

void gameLoop() {
    bool quit = false;
    SDL_Event event;

    const int frameDelay = 1000 / FPS;

    // Main game loop
    while (!quit) {
        Uint32 frameStart = SDL_GetTicks();

        // Handle events
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            sceneManager_handleEvents(&event);
        }

        // Update and render
        sceneManager_update();
        sceneManager_render();

        // Frame rate cap
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    gameQuit();
}
