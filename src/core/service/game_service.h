#pragma once

#include "model/game.h"
#include "utils/event_system.h"

typedef struct {
    bool isGameWon;
    GamePhase currentPhase;
} GameView;

void gameService_init(GameState* initialState);
void gameService_cleanup();
void gameService_handleDeckOperation(EventType operation, void* data);

void gameService_moveCard(Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex);
void gameService_loadDeck(const char* filePath);
void gameService_saveDeck(const char* filePath);
void gameService_toggleShowDeck();
void gameService_shuffleDeck();
void gameService_splitDeck(int splitIndex);
void gameService_checkGameWon();

const GameView* gameService_getView();
GameState* gameService_getGameState();