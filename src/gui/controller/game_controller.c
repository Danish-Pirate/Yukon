#include "SDL2/SDL.h"
#include "game_controller.h"
#include "scene/scene_manager.h"
#include "view/texture_manager.h"
#include "nativefiledialog-extended/src/include/nfd.h"
#include "../utils/service_locator.h"
#include "../utils/error_handler.h"
#include "../service/core_service.h"
#include "view/window_manager.h"
#include "SDL_ttf.h"
#include "view/ui_manager.h"

#define ERROR_DIALOG_TIMEOUT 3000 // 3 seconds

typedef struct {
    char message[256];
    Uint32 showUntil;
    bool isVisible;
} ErrorDialog;

static ErrorDialog errorDialog = {0};

static void handleDeckOperation(Event* event) {
    if (event) {
        gameService_handleDeckOperation(event->type, event->data);
    }
}

static void handleErrorEvent(ErrorCode code, const char* message) {
    // Log error to console
    fprintf(stderr, "Error occurred [%d]: %s\n", code, message);

    // Set up error dialog
    strncpy(errorDialog.message, message, sizeof(errorDialog.message) - 1);
    errorDialog.showUntil = SDL_GetTicks() + ERROR_DIALOG_TIMEOUT;
    errorDialog.isVisible = true;
}

static void renderErrorDialog() {
    if (!errorDialog.isVisible) return;

    // Check if it's time to hide the dialog
    if (SDL_GetTicks() > errorDialog.showUntil) {
        errorDialog.isVisible = false;
        return;
    }

    SDL_Renderer* renderer = windowManager_getRenderer();
    if (!renderer) return;

    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    // Semi-transparent background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect bgRect = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &bgRect);

    // Error dialog
    int dialogWidth = 500;
    int dialogHeight = 200;
    SDL_Rect dialogRect = {
            (width - dialogWidth) / 2,
            (height - dialogHeight) / 2,
            dialogWidth,
            dialogHeight
    };

    SDL_SetRenderDrawColor(renderer, 220, 100, 100, 255);
    SDL_RenderFillRect(renderer, &dialogRect);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderDrawRect(renderer, &dialogRect);

    // Render error message
    TTF_Font* font = TTF_OpenFont(FONT_FILEPATH, 18);
    if (font) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font,
                                                                  errorDialog.message,
                                                                  textColor,
                                                                  dialogWidth - 40);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                SDL_Rect textRect = {
                        dialogRect.x + 20,
                        dialogRect.y + 20,
                        textSurface->w,
                        textSurface->h
                };
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }

        // Render instruction
        const char* instruction = "Click anywhere to dismiss";
        SDL_Surface* instructionSurface = TTF_RenderText_Solid(font, instruction, textColor);
        if (instructionSurface) {
            SDL_Texture* instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
            if (instructionTexture) {
                SDL_Rect instructionRect = {
                        dialogRect.x + (dialogWidth - instructionSurface->w) / 2,
                        dialogRect.y + dialogHeight - instructionSurface->h - 20,
                        instructionSurface->w,
                        instructionSurface->h
                };
                SDL_RenderCopy(renderer, instructionTexture, NULL, &instructionRect);
                SDL_DestroyTexture(instructionTexture);
            }
            SDL_FreeSurface(instructionSurface);
        }

        TTF_CloseFont(font);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

static bool handleErrorDialogEvents(SDL_Event* event) {
    if (!errorDialog.isVisible) return false;

    if (event->type == SDL_MOUSEBUTTONDOWN) {
        errorDialog.isVisible = false;
        return true;
    }

    return false;
}

void initGameController(SDL_Window *window, SDL_Renderer *renderer) {
    // Make sure we're using only the GUI service locator
    serviceLocator_init();
    yukon_eventSystem_init();

    windowManager_init(window, renderer);

    errorHandler_subscribe(handleErrorEvent);

    coreService_init();

    GameState* gameState = coreService_getGameState();
    if (gameState) {
        fprintf(stderr, "DEBUG: Registering gameState with GUI service locator\n");
        serviceLocator_registerGameState(gameState);

    } else {
        fprintf(stderr, "ERROR: coreService_getGameState() returned NULL\n");
    }

    coreService_subscribeToEvents();

    sceneManager_init();
    textureManager_init();

    sceneManager_subscribeToEvents();
    yukon_eventSystem_subscribe(EVENT_DECK_SHUFFLED, handleDeckOperation);
    yukon_eventSystem_subscribe(EVENT_DECK_TOGGLED, handleDeckOperation);
    yukon_eventSystem_subscribe(EVENT_DECK_SPLIT, handleDeckOperation);

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

            // Handle error dialog events first
            if (!handleErrorDialogEvents(&event)) {
                sceneManager_handleEvents(&event);
            }
        }

        sceneManager_update();

        // Render the scene
        SDL_Renderer* renderer = windowManager_getRenderer();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        sceneManager_render();

        // Render error dialog on top if visible
        renderErrorDialog();

        SDL_RenderPresent(renderer);

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void cleanUpGameController() {
    errorHandler_unsubscribe(handleErrorEvent);
    yukon_eventSystem_unsubscribe(EVENT_DECK_SHUFFLED, handleDeckOperation);
    yukon_eventSystem_unsubscribe(EVENT_DECK_TOGGLED, handleDeckOperation);
    yukon_eventSystem_unsubscribe(EVENT_DECK_SPLIT, handleDeckOperation);

    coreService_cleanup();
    yukon_eventSystem_cleanup();
    textureManager_cleanup();
    sceneManager_cleanup();
    serviceLocator_cleanup();
}