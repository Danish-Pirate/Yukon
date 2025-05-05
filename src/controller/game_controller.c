// src/controller/game_controller.c
#include <SDL.h>
#include <stdio.h>
#include "game_controller.h"
#include "scene_manager.h"
#include "texture_manager.h"
#include "nfd.h"
#include "../utils/event_system.h"

// Private game state
static GameState* gameState = NULL;

// Event handlers
static void handleSceneChangeEvent(Event* event);
static void handlePlayModeEnterEvent(Event* event);
static void handlePlayModeExitEvent(Event* event);
static void handleCardMovedEvent(Event* event);
static void handleGameWonEvent(Event* event);
static void handleDeckShuffledEvent(Event* event);

static void subscribeToEvents() {
    eventSystem_subscribe(EVENT_SCENE_CHANGE, handleSceneChangeEvent);
    eventSystem_subscribe(EVENT_PLAY_MODE_ENTER, handlePlayModeEnterEvent);
    eventSystem_subscribe(EVENT_PLAY_MODE_EXIT, handlePlayModeExitEvent);
    eventSystem_subscribe(EVENT_CARD_MOVED, handleCardMovedEvent);
    eventSystem_subscribe(EVENT_GAME_WON, handleGameWonEvent);
    eventSystem_subscribe(EVENT_DECK_SHUFFLED, handleDeckShuffledEvent);
}

// Initialize the game
void gameController_init() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // Initialize event system
    eventSystem_init();

    // Create window
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

    // Initialize scene manager
    sceneManager_init(window, renderer);

    // Initialize texture manager
    textureManager_init();

    // Initialize game state
    gameState = initGame();

    // Subscribe to events
    subscribeToEvents();

    SceneChangeData* data = malloc(sizeof(SceneChangeData));
    data->type = SCENE_STARTUP_MODE;
    data->data = NULL;

    eventSystem_publish(EVENT_SCENE_CHANGE, data);
}

// Game loop
void gameController_loop() {
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

    gameController_cleanup();
}

// Cleanup resources
void gameController_cleanup() {
    // Clean up event system
    eventSystem_cleanup();

    // Clean up textures
    textureManager_cleanup();

    // Clean up the scene manager resources
    sceneManager_cleanup();

    // Clean up game state
    if (gameState != NULL) {
        // Free game state memory
        free(gameState);
        gameState = NULL;
    }
}

// Event handler implementations
static void handleSceneChangeEvent(Event* event) {
    if (event->type != EVENT_SCENE_CHANGE) return;

    // This is handled by the scene manager, which should subscribe to this event
    // No need to do anything here, just demonstrating the pattern
}

static void handlePlayModeEnterEvent(Event* event) {
    if (event->type != EVENT_PLAY_MODE_ENTER) return;

    enterPlayMode(gameState);
}

static void handlePlayModeExitEvent(Event* event) {
    if (event->type != EVENT_PLAY_MODE_EXIT) return;

    exitPlayMode(gameState);
}

static void handleCardMovedEvent(Event* event) {
    if (event->type != EVENT_CARD_MOVED) return;

    // Extract move data from event and call gameManager_moveCard
    typedef struct {
        Rank rank;
        Suit suit;
        int fromColumnIndex;
        int toColumnIndex;
    } CardMoveData;

    CardMoveData* moveData = (CardMoveData*)event->data;

    moveCard(gameState,
                         moveData->rank,
                         moveData->suit,
                         moveData->fromColumnIndex,
                         moveData->toColumnIndex);

    free(moveData);

    // Check if game is won after move
    isGameWon(gameState);
}

static void handleGameWonEvent(Event* event) {
    if (event->type != EVENT_GAME_WON) return;

    // Handle game won - could trigger effects, sounds, etc.
    printf("Game Won!\n");
}

static void handleDeckShuffledEvent(Event* event) {
    if (event->type != EVENT_DECK_SHUFFLED) return;

    // Handle deck shuffled - could trigger animations, etc.
    printf("Deck Shuffled!\n");
}

// Game state accessor - this will eventually be replaced by events
GameState* gameController_getGameState() {
    return gameState;
}