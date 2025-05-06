#include "game_service.h"
#include "../utils/error_handler.h"
#include <stdlib.h>

static GameState* gameState = NULL;
static GameView currentView = {0};

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

    CardMoveData* moveData = (CardMoveData*)event->data;
    moveCard(gameState, moveData->rank, moveData->suit,
             moveData->fromColumnIndex, moveData->toColumnIndex);

    isGameWon(gameState);
    currentView.isGameWon = gameState->gameWon;

    if (gameState->gameWon) {
        eventSystem_publish(EVENT_GAME_WON, NULL);
    }

    free(moveData);
}

void gameService_init(GameState* initialState) {
    gameState = initialState;
    currentView.isGameWon = gameState->gameWon;
    currentView.currentPhase = gameState->gamePhase;

    eventSystem_subscribe(EVENT_PLAY_MODE_ENTER, handlePlayModeEnterEvent);
    eventSystem_subscribe(EVENT_PLAY_MODE_EXIT, handlePlayModeExitEvent);
    eventSystem_subscribe(EVENT_CARD_MOVED, handleCardMovedEvent);
}

void gameService_cleanup() {
    gameState = NULL;
    eventSystem_unsubscribe(EVENT_PLAY_MODE_ENTER, handlePlayModeEnterEvent);
    eventSystem_unsubscribe(EVENT_PLAY_MODE_EXIT, handlePlayModeExitEvent);
    eventSystem_unsubscribe(EVENT_CARD_MOVED, handleCardMovedEvent);
}

void gameService_handleDeckOperation(EventType operation, void* data) {
    switch(operation) {
        case EVENT_DECK_SHUFFLED:
            randomShuffleDeck(gameState->deck);
            break;
        case EVENT_DECK_TOGGLED:
            toggleShowDeck(gameState->deck);
            break;
        case EVENT_DECK_SPLIT:
            if (data) {
                int splitIndex = *(int*)data;
                splitDeck(gameState->deck, splitIndex);
            } else {
                splitDeck(gameState->deck, -1);
            }
            break;
        default:
            break;
    }

    currentView.isGameWon = gameState->gameWon;
    currentView.currentPhase = gameState->gamePhase;
}

void gameService_moveCard(Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex) {
    CardMoveData* moveData = malloc(sizeof(CardMoveData));
    if (!moveData) {
        errorHandler_reportError(ERROR_MEMORY_ALLOCATION, "Failed to allocate memory for card move data");
        return;
    }

    moveData->rank = rank;
    moveData->suit = suit;
    moveData->fromColumnIndex = fromColumnIndex;
    moveData->toColumnIndex = toColumnIndex;

    eventSystem_publish(EVENT_CARD_MOVED, moveData);
}

void gameService_loadDeck(const char* filePath) {
    if (!filePath) {
        errorHandler_reportError(ERROR_INVALID_PARAMETER, "Null file path provided to loadDeck");
        return;
    }

    DeckLoadResult result = loadDeckFromFile(&gameState->deck, filePath);
    EventType eventType = (result == LOAD_SUCCESS) ?
                          EVENT_DECK_LOADED_SUCCESS :
                          EVENT_DECK_LOADED_FAILURE;

    if (result != LOAD_SUCCESS) {
        char errorMsg[100];
        sprintf(errorMsg, "Failed to load deck from file: %s, error code: %d", filePath, result);
        errorHandler_reportError(ERROR_FILE_IO, errorMsg);
    }

    eventSystem_publish(eventType, NULL);
}

void gameService_saveDeck(const char* filePath) {
    if (!filePath) {
        errorHandler_reportError(ERROR_INVALID_PARAMETER, "Null file path provided to saveDeck");
        return;
    }

    saveDeckToFile(gameState->deck, filePath);
    eventSystem_publish(EVENT_DECK_SAVED, NULL);
}

void gameService_toggleShowDeck() {
    eventSystem_publish(EVENT_DECK_TOGGLED, NULL);
}

void gameService_shuffleDeck() {
    eventSystem_publish(EVENT_DECK_SHUFFLED, NULL);
}

void gameService_splitDeck(int splitIndex) {
    int* data = malloc(sizeof(int));
    if (!data) {
        errorHandler_reportError(ERROR_MEMORY_ALLOCATION, "Failed to allocate memory for split index");
        return;
    }

    *data = splitIndex;
    eventSystem_publish(EVENT_DECK_SPLIT, data);
}

void gameService_checkGameWon() {
    isGameWon(gameState);

    if (gameState->gameWon) {
        eventSystem_publish(EVENT_GAME_WON, NULL);
    }
}

const GameView* gameService_getView() {
    currentView.isGameWon = gameState->gameWon;
    currentView.currentPhase = gameState->gamePhase;
    return &currentView;
}

GameState* gameService_getGameState() {
    return gameState;
}