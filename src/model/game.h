#include "card.h"
#include "game_utils.h"
#include "../utils/linked_list.h"

#ifndef YUKON_CARDGAME_GAME_MANAGER_H
#define YUKON_CARDGAME_GAME_MANAGER_H

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
void resetGameState(GameState* gameState);
void gameManager_randomShuffleDeck(GameState* gameState);
void gameManager_saveDeckToFile(GameState* gameState, char filepath[100]);
void gameManager_quitProgram(GameState* gameState);
void gameManager_enterPlayMode(GameState* gameState);
void gameManager_exitPlayMode(GameState* gameState);
void gameManager_moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex);
void gameManager_isGameWon(GameState* gameState);


#endif //YUKON_CARDGAME_GAME_MANAGER_H
