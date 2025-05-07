#include <SDL.h>
#include "game_controller.h"
#include "scene/scene_manager.h"
#include "view/texture_manager.h"
#include "nativefiledialog-extended/src/include/nfd.h"
#include "view/window_manager.h"
#include "service/core_service.h"
#include "utils/error_handler.h"


// Central event handler for all game events
static void handleGameEvents(Event* event) {
    if (!event) {
        fprintf(stderr, "ERROR: Null event in handleGameEvents\n");
        return;
    }

    switch (event->type) {
        case EVENT_SCENE_CHANGE:
            if (event->data) {
                SceneChangeData* data = (SceneChangeData*)event->data;
                sceneManager_changeScene(data->type, data->data);
            }
            break;

        case EVENT_GAME_INITIALIZED:
            break;

        case EVENT_GAME_WON:
            break;

        case EVENT_PLAY_MODE_ENTER:
            coreService_enterPlayMode();
            break;

        case EVENT_PLAY_MODE_EXIT:
            coreService_exitPlayMode();
            break;

        case EVENT_CARD_MOVED:
            if (event->data) {
                CardMoveData* moveData = (CardMoveData*)event->data;
                coreService_moveCard(moveData->rank, moveData->suit,
                                     moveData->fromColumnIndex, moveData->toColumnIndex);

                if (coreService_isGameWon()) {
                    eventSystem_publish(EVENT_GAME_WON, NULL);
                }

                free(moveData);
            } else {
                fprintf(stderr, "ERROR: EVENT_CARD_MOVED received null data\n");
            }
            break;

        case EVENT_DECK_SHUFFLED:
            coreService_shuffleDeck();
            break;

        case EVENT_DECK_LOADED_SUCCESS:
            if (event->data) {
                const char* filePath = (const char*)event->data;
                if (coreService_loadDeck(filePath)) {
                    coreService_toggleShowDeck();
                } else {
                    errorHandler_reportError(ERROR_FILE_IO, "Failed to load deck");
                    eventSystem_publish(EVENT_DECK_LOADED_FAILURE, NULL);
                }
            } else {
                fprintf(stderr, "ERROR: EVENT_DECK_LOADED_SUCCESS received null path\n");
            }
            break;

        case EVENT_DECK_LOADED_FAILURE:
            errorHandler_reportError(ERROR_FILE_IO, "Failed to load deck");
            break;

        case EVENT_DECK_SAVED:
            if (event->data) {
                const char* filePath = (const char*)event->data;
                coreService_saveDeck(filePath);
            } else {
                fprintf(stderr, "ERROR: EVENT_DECK_SAVED received null path\n");
            }
            break;

        case EVENT_DECK_TOGGLED:
            coreService_toggleShowDeck();
            break;

        case EVENT_DECK_SHOWN:
            coreService_showDeck();
            break;

        case EVENT_DECK_SPLIT:
            if (event->data) {
                int* splitIndex = (int*)event->data;
                coreService_splitDeck(*splitIndex);
                free(splitIndex);
            } else {
                coreService_splitDeck(-1);
            }
            break;

        default:
            fprintf(stderr, "ERROR: Unhandled event type in handleGameEvents: %d\n", event->type);
            break;
    }
}

// Initialize the game controller
void initGameController(SDL_Window *window, SDL_Renderer *renderer) {
    windowManager_init(window, renderer);

    yukon_eventSystem_init();
    coreService_init();
    sceneManager_init();
    textureManager_init();

    coreService_subscribeToEvents();
    // Subscribe to ALL events for centralized handling
    yukon_eventSystem_subscribe(EVENT_SCENE_CHANGE, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_GAME_INITIALIZED, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_GAME_WON, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_PLAY_MODE_ENTER, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_PLAY_MODE_EXIT, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_CARD_MOVED, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_DECK_SHUFFLED, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_DECK_LOADED_SUCCESS, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_DECK_LOADED_FAILURE, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_DECK_SAVED, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_DECK_TOGGLED, handleGameEvents);
    yukon_eventSystem_subscribe(EVENT_DECK_SPLIT, handleGameEvents);

    sceneManager_changeScene(SCENE_STARTUP_MODE, NULL);
}

// Main game loop
void loopGameController() {
    bool quit = false;
    SDL_Renderer* renderer = windowManager_getRenderer();
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

        // Render the scene
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            sceneManager_render();

            SDL_RenderPresent(renderer);
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void cleanUpGameController() {
    yukon_eventSystem_unsubscribe(EVENT_SCENE_CHANGE, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_GAME_INITIALIZED, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_GAME_WON, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_PLAY_MODE_ENTER, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_PLAY_MODE_EXIT, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_CARD_MOVED, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_DECK_SHUFFLED, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_DECK_LOADED_SUCCESS, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_DECK_LOADED_FAILURE, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_DECK_SAVED, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_DECK_TOGGLED, handleGameEvents);
    yukon_eventSystem_unsubscribe(EVENT_DECK_SPLIT, handleGameEvents);

    coreService_cleanup();
    yukon_eventSystem_cleanup();
    textureManager_cleanup();
    sceneManager_cleanup();
}