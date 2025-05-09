#include "core_service.h"
#include "model/card.h"
#include "utils/event_system.h"
#include <stdio.h>

static GameState* gameState = NULL;

GameState* coreService_getGameState() {
    return gameState;
}

void coreService_init() {
    gameState = yukon_initGame();
    if (!gameState) {
        fprintf(stderr, "Failed to initialize game state in core service\n");
    }
}

void coreService_cleanup() {
    if (gameState) {
        yukon_cleanup(gameState);
        gameState = NULL;
    }
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

int coreService_getDeckSize() {
    if (gameState) {
        return yukon_getDeckSize(gameState);
    }
    return 0;
}

void coreService_showDeck() {
    yukon_showDeck(gameState);
}

Card* coreService_getDeckCard(int index) {
    if (gameState) {
        return yukon_getDeckCard(gameState, index);
    }
    return NULL;
}