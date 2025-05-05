#pragma once

#include "card.h"
#include "../utils/game_utils.h"
#include "../utils/linked_list.h"

#define DECK_SIZE 52 // Size of card deck
#define COLUMNS_SIZE 7  // Number of card columns
#define PILES_SIZE 4    // Number of card foundation piles

typedef enum {
    StartupPhase,
    PlayPhase,
} GamePhase;

typedef struct {
    LinkedList *cardColumns[COLUMNS_SIZE];
    LinkedList *cardFoundationPiles[PILES_SIZE];
    LinkedList *deck;
    bool gameWon;
    GamePhase gamePhase;
    char lastCommand[100];
    char lastResponse[50];
} GameState;

GameState *initGame();
void resetGame(GameState* gameState);
void quitGame(GameState* gameState);
void enterPlayMode(GameState* gameState);
void exitPlayMode(GameState* gameState);
void moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex);
void isGameWon(GameState* gameState);
