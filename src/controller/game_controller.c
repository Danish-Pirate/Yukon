#include <SDL.h>
#include "game_controller.h"
#include "../scene_manager.h"
#include "../texture_manager.h"
#include <nfd.h>
#include "../service/game_service.h"
#include "../utils/service_locator.h"
#include "../utils/error_handler.h"
#include "../view/window.h"

static void handleDeckOperation(Event* event) {
    if (event) {
        gameService_handleDeckOperation(event->type, event->data);
    }
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

    sceneManager_subscribeToEvents();
    eventSystem_subscribe(EVENT_DECK_SHUFFLED, handleDeckOperation);
    eventSystem_subscribe(EVENT_DECK_TOGGLED, handleDeckOperation);
    eventSystem_subscribe(EVENT_DECK_SPLIT, handleDeckOperation);

    sceneManager_changeScene(SCENE_STARTUP_MODE, NULL);
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