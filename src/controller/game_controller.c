#include <SDL.h>
#include "game_controller.h"
#include "../scene_manager.h"
#include "../texture_manager.h"
#include <nfd.h>
#include "../service/game_service.h"
#include "../utils/service_locator.h"
#include "../utils/error_handler.h"

static void handleSceneChangeEvent(Event* event) {
    if (event->type != EVENT_SCENE_CHANGE) return;

    SceneChangeData* data = (SceneChangeData*)event->data;
    sceneManager_changeScene(data->type, data->data);
    free(data);
}

static void handleErrorEvent(ErrorCode code, const char* message) {
    // Log error to console
    fprintf(stderr, "Error occurred [%d]: %s\n", code, message);
}

void initGameController(SDL_Window *window, SDL_Renderer *renderer) {
    serviceLocator_init();
    eventSystem_init();

    serviceLocator_registerWindow(window);
    serviceLocator_registerRenderer(renderer);

    errorHandler_subscribe(handleErrorEvent);

    GameState* gameState = initGame();
    if (!gameState) {
        errorHandler_reportError(ERROR_MEMORY_ALLOCATION, "Failed to initialize game state");
        return;
    }

    serviceLocator_registerGameState(gameState);
    gameService_init(gameState);

    sceneManager_init();
    textureManager_init();

    eventSystem_subscribe(EVENT_SCENE_CHANGE, handleSceneChangeEvent);
    eventSystem_subscribe(EVENT_DECK_SHUFFLED, gameService_handleDeckOperation);
    eventSystem_subscribe(EVENT_DECK_TOGGLED, gameService_handleDeckOperation);
    eventSystem_subscribe(EVENT_DECK_SPLIT, gameService_handleDeckOperation);

    SceneChangeData* data = malloc(sizeof(SceneChangeData));
    if (!data) {
        errorHandler_reportError(ERROR_MEMORY_ALLOCATION, "Failed to allocate memory for scene change data");
        return;
    }

    data->type = SCENE_STARTUP_MODE;
    data->data = NULL;
    eventSystem_publish(EVENT_SCENE_CHANGE, data);
}

void loopGameController() {
    bool quit = false;
    SDL_Event event;
    const int frameDelay = 1000 / FPS;

    while (!quit) {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            sceneManager_handleEvents(&event);
        }

        sceneManager_update();
        sceneManager_render();

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void cleanUpGameController() {
    errorHandler_unsubscribe(handleErrorEvent);
    gameService_cleanup();
    eventSystem_cleanup();
    textureManager_cleanup();
    sceneManager_cleanup();
    serviceLocator_cleanup();
}