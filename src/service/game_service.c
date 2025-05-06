#include "game_service.h"
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
        default:
            break;
    }

    currentView.isGameWon = gameState->gameWon;
    currentView.currentPhase = gameState->gamePhase;
}

void gameService_moveCard(Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex) {
    CardMoveData* moveData = malloc(sizeof(CardMoveData));
    moveData->rank = rank;
    moveData->suit = suit;
    moveData->fromColumnIndex = fromColumnIndex;
    moveData->toColumnIndex = toColumnIndex;

    eventSystem_publish(EVENT_CARD_MOVED, moveData);
}

void gameService_loadDeck(const char* filePath) {
    DeckLoadResult result = loadDeckFromFile(&gameState->deck, filePath);

    EventType eventType = (result == LOAD_SUCCESS) ?
                          EVENT_DECK_LOADED_SUCCESS :
                          EVENT_DECK_LOADED_FAILURE;

    eventSystem_publish(eventType, NULL);
}

void gameService_saveDeck(const char* filePath) {
    saveDeckToFile(gameState->deck, filePath);
    eventSystem_publish(EVENT_DECK_SAVED, NULL);
}

void gameService_toggleShowDeck() {
    toggleShowDeck(gameState->deck);
    eventSystem_publish(EVENT_DECK_TOGGLED, NULL);
}

void gameService_shuffleDeck() {
    eventSystem_publish(EVENT_DECK_SHUFFLED, NULL);
}

void gameService_splitDeck(int splitIndex) {
    splitDeck(gameState->deck, splitIndex);
    eventSystem_publish(EVENT_DECK_SPLIT, NULL);
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