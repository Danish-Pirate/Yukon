// In game_service.c
#include "game_service.h"
#include <stdlib.h>

// Store the game state pointer
static GameState* gameState = NULL;
static GameView currentView = {0};

// Forward declarations for event handlers
static void handlePlayModeEnterEvent(Event* event);
static void handlePlayModeExitEvent(Event* event);
static void handleCardMovedEvent(Event* event);

void gameService_init(GameState* initialState) {
    // Store reference to the state (don't create your own)
    gameState = initialState;

    // Initialize the view
    currentView.isGameWon = gameState->gameWon;
    currentView.currentPhase = gameState->gamePhase;

    // Subscribe to relevant events
    eventSystem_subscribe(EVENT_PLAY_MODE_ENTER, handlePlayModeEnterEvent);
    eventSystem_subscribe(EVENT_PLAY_MODE_EXIT, handlePlayModeExitEvent);
    eventSystem_subscribe(EVENT_CARD_MOVED, handleCardMovedEvent);
}

void gameService_cleanup() {
    // Don't free gameState, just clear the reference
    gameState = NULL;

    // Unsubscribe from events
    eventSystem_unsubscribe(EVENT_PLAY_MODE_ENTER, handlePlayModeEnterEvent);
    eventSystem_unsubscribe(EVENT_PLAY_MODE_EXIT, handlePlayModeExitEvent);
    eventSystem_unsubscribe(EVENT_CARD_MOVED, handleCardMovedEvent);
}

void gameService_handleDeckOperation(EventType operation, void* data) {
    // Handle the different deck operations
    switch(operation) {
        case EVENT_DECK_SHUFFLED:
            randomShuffleDeck(gameState->deck);
            break;
            // Add other operations as needed
        default:
            break;
    }

    // Update the view after operation
    currentView.isGameWon = gameState->gameWon;
    currentView.currentPhase = gameState->gamePhase;
}

const GameView* gameService_getView() {
    // Update the view with current state
    currentView.isGameWon = gameState->gameWon;
    currentView.currentPhase = gameState->gamePhase;

    return &currentView;
}

// Event handler implementations
static void handlePlayModeEnterEvent(Event* event) {
    if (event->type != EVENT_PLAY_MODE_ENTER) return;

    enterPlayMode(gameState);
    currentView.currentPhase = gameState->gamePhase;
}

static void handlePlayModeExitEvent(Event* event) {
    if (event->type != EVENT_PLAY_MODE_EXIT) return;

    exitPlayMode(gameState);
    currentView.currentPhase = gameState->gamePhase;
}

static void handleCardMovedEvent(Event* event) {
    if (event->type != EVENT_CARD_MOVED) return;

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

    // Check if the game is won after a move
    isGameWon(gameState);
    currentView.isGameWon = gameState->gameWon;

    if (gameState->gameWon) {
        eventSystem_publish(EVENT_GAME_WON, NULL);
    }

    free(moveData);
}