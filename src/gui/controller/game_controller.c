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

    // Handle all possible event types
    switch (event->type) {
        // Scene management
        case EVENT_SCENE_CHANGE:
            if (event->data) {
                SceneChangeData* data = (SceneChangeData*)event->data;
                sceneManager_changeScene(data->type, data->data);
            }
            break;
            // Game state events
        case EVENT_GAME_INITIALIZED:
            // Handle game initialization if needed
            break;
        case EVENT_GAME_WON:
            // The UI will handle showing the victory dialog
            break;
        case EVENT_PLAY_MODE_ENTER:
            coreService_enterPlayMode();
            break;
        case EVENT_PLAY_MODE_EXIT:
            coreService_exitPlayMode();
            break;
            // Card operations
        case EVENT_CARD_MOVED:
            if (event->data) {
                CardMoveData* moveData = (CardMoveData*)event->data;
                coreService_moveCard(moveData->rank, moveData->suit,
                                     moveData->fromColumnIndex, moveData->toColumnIndex);
                // Check if game is won after a card move
                if (coreService_isGameWon()) {
                    eventSystem_publish(EVENT_GAME_WON, NULL);
                }
            }
            break;
            // Deck operations
        case EVENT_DECK_SHUFFLED:
            coreService_shuffleDeck();
            break;
        case EVENT_DECK_LOADED_SUCCESS:
            // Handle successful deck loading (update UI, etc.)
            break;
        case EVENT_DECK_LOADED_FAILURE:
            // Error will be shown through error dialog system
            errorHandler_reportError(ERROR_FILE_IO, "Failed to load deck");
            break;
        case EVENT_DECK_SAVED:
            // Handle successful save (update UI, etc.)
            break;
        case EVENT_DECK_TOGGLED:
            coreService_toggleShowDeck();
            break;
        case EVENT_DECK_SPLIT: {
            int* splitIndex = (int*)event->data;
            if (splitIndex != NULL) {
                coreService_splitDeck(*splitIndex);
                free(splitIndex);
            } else {
                fprintf(stderr, "Error: EVENT_DECK_SPLIT received null data.\n");
            }
            break;
        }
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