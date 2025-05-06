#include <SDL.h>
#include <stdio.h>
#include "game_controller.h"
#include "scene_manager.h"
#include "texture_manager.h"
#include <nfd.h>
#include "../service/game_service.h"
#include "../utils/service_locator.h"

static void handleSceneChangeEvent(Event* event) {
    if (event->type != EVENT_SCENE_CHANGE) return;

    SceneChangeData* data = (SceneChangeData*)event->data;
    sceneManager_changeScene(data->type, data->data);
    free(data);
}
void initGameController(SDL_Window *window, SDL_Renderer *renderer) {
    serviceLocator_init();
    eventSystem_init();
    serviceLocator_registerWindow(window);
    serviceLocator_registerRenderer(renderer);
    sceneManager_init(window, renderer);  // Will be updated to use service locator
    textureManager_init();  // Will be updated to use service locator

    gameState = initGame();
    serviceLocator_registerGameState(gameState);

    subscribeToEvents();

    SceneChangeData* data = malloc(sizeof(SceneChangeData));
    data->type = SCENE_STARTUP_MODE;
    data->data = NULL;
    eventSystem_publish(EVENT_SCENE_CHANGE, data);
}

void loopGameController() {
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

        sceneManager_update();
        sceneManager_render();

        // Frame rate cap
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void cleanUpGameController() {
    gameService_cleanup();
    eventSystem_cleanup();
    textureManager_cleanup();
    sceneManager_cleanup();
    if (gameState != NULL) {
        free(gameState);
        gameState = NULL;
    }
}