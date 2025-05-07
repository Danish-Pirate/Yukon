#include "include/yukon_core.h"
#include "model/game.h"
#include "model/card.h"
#include "model/deck.h"
#include "utils/event_system.h"
#include <stdlib.h>
#include <string.h>

GameState* yukon_initGame() {
    return initGame();
}

void yukon_resetGame(GameState* gameState) {
    resetGame(gameState);
}

void yukon_cleanup(GameState* gameState) {
    // Free game state resources
    if (gameState) {
        // Clean up lists
        for (int i = 0; i < COLUMNS_SIZE; i++) {
            if (gameState->cardColumns[i]) {
                freeList(gameState->cardColumns[i]);
            }
        }

        for (int i = 0; i < PILES_SIZE; i++) {
            if (gameState->cardFoundationPiles[i]) {
                freeList(gameState->cardFoundationPiles[i]);
            }
        }

        if (gameState->deck) {
            freeList(gameState->deck);
        }

        free(gameState);
    }
}

void yukon_eventSystem_init() {
    eventSystem_init();
}

void yukon_eventSystem_cleanup() {
    eventSystem_cleanup();
}

bool yukon_eventSystem_subscribe(EventType type, EventCallback callback) {
    return eventSystem_subscribe(type, callback);
}

bool yukon_eventSystem_unsubscribe(EventType type, EventCallback callback) {
    return eventSystem_unsubscribe(type, callback);
}

void yukon_eventSystem_publish(EventType type, void* data) {
    eventSystem_publish(type, data);
}

void yukon_enterPlayMode(GameState* gameState) {
    enterPlayMode(gameState);
}

void yukon_exitPlayMode(GameState* gameState) {
    exitPlayMode(gameState);
}

void yukon_moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex) {
    moveCard(gameState, rank, suit, fromColumnIndex, toColumnIndex);
}

bool yukon_isGameWon(GameState* gameState) {
    isGameWon(gameState);
    return gameState->gameWon;
}

void yukon_saveDeck(GameState* gameState, const char* filePath) {
    saveDeckToFile(gameState->deck, filePath);
}

bool yukon_loadDeck(GameState* gameState, const char* filePath) {
    return loadDeckFromFile(&gameState->deck, filePath) == LOAD_SUCCESS;
}

void yukon_toggleShowDeck(GameState* gameState) {
    toggleShowDeck(gameState->deck);
}

void yukon_shuffleDeck(GameState* gameState) {
    randomShuffleDeck(gameState->deck);
}

void yukon_splitDeck(GameState* gameState, int splitIndex) {
    splitDeck(gameState->deck, splitIndex);
}

// New API functions implementation
const GameView* yukon_getGameView(GameState* gameState) {
    static GameView view;
    view.isGameWon = gameState->gameWon;
    view.currentPhase = gameState->gamePhase;
    return &view;
}

bool yukon_getIsGameWon(const GameView* view) {
    return view->isGameWon;
}

GamePhase yukon_getGamePhase(const GameView* view) {
    return view->currentPhase;
}

Card* yukon_getCard(GameState* gameState, int columnIndex, int cardIndex) {
    if (columnIndex >= 0 && columnIndex < COLUMNS_SIZE) {
        LinkedList* column = gameState->cardColumns[columnIndex];
        Node* node = getNode(column, cardIndex);
        return node ? node->data : NULL;
    }
    return NULL;
}

const char* yukon_cardToString(Card* card) {
    return cardToString(card);
}

bool yukon_isCardFaceUp(Card* card) {
    return card ? card->isFaceUp : false;
}

Rank yukon_getCardRank(Card* card) {
    return card ? card->rank : INVALID_RANK;
}

Suit yukon_getCardSuit(Card* card) {
    return card ? card->suit : INVALID_SUIT;
}