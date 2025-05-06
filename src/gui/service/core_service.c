#include "core_service.h"
#include "utils/gui_service_locator.h"
#include "model/card.h"
#include <stdio.h>

static GameState* gameState = NULL;

void coreService_init() {
    gameState = yukon_initGame();
    if (!gameState) {
        fprintf(stderr, "Failed to initialize game state in core service\n");
    }
    serviceLocator_registerGameState(gameState);
}

void coreService_cleanup() {
    if (gameState) {
        yukon_cleanup(gameState);
        gameState = NULL;
    }
}

void coreService_subscribeToEvents() {
    // Subscribe to events from the core
    yukon_eventSystem_init();
}

void coreService_enterPlayMode() {
    if (gameState) {
        yukon_enterPlayMode(gameState);
    }
}

void coreService_exitPlayMode() {
    if (gameState) {
        yukon_exitPlayMode(gameState);
    }
}

const GameView* coreService_getGameView() {
    if (gameState) {
        return yukon_getGameView(gameState);
    }
    return NULL;
}

bool coreService_isGameWon() {
    if (gameState) {
        return yukon_isGameWon(gameState);
    }
    return false;
}

GamePhase coreService_getGamePhase() {
    const GameView* view = coreService_getGameView();
    if (view) {
        return yukon_getGamePhase(view);
    }
    return StartupPhase;
}

void coreService_moveCard(Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex) {
    if (gameState) {
        yukon_moveCard(gameState, rank, suit, fromColumnIndex, toColumnIndex);
    }
}

Card* coreService_getCard(int columnIndex, int cardIndex) {
    if (gameState) {
        return yukon_getCard(gameState, columnIndex, cardIndex);
    }
    return NULL;
}

bool coreService_isCardFaceUp(Card* card) {
    return yukon_isCardFaceUp(card);
}

Rank coreService_getCardRank(Card* card) {
    return yukon_getCardRank(card);
}

Suit coreService_getCardSuit(Card* card) {
    return yukon_getCardSuit(card);
}

const char* coreService_cardToString(Card* card) {
    return yukon_cardToString(card);
}

void coreService_saveDeck(const char* filePath) {
    if (gameState) {
        yukon_saveDeck(gameState, filePath);
    }
}

bool coreService_loadDeck(const char* filePath) {
    if (gameState) {
        return yukon_loadDeck(gameState, filePath);
    }
    return false;
}

void coreService_toggleShowDeck() {
    if (gameState) {
        yukon_toggleShowDeck(gameState);
    }
}

void coreService_shuffleDeck() {
    if (gameState) {
        yukon_shuffleDeck(gameState);
    }
}

void coreService_splitDeck(int splitIndex) {
    if (gameState) {
        yukon_splitDeck(gameState, splitIndex);
    }
}