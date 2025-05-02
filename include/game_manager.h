//
// Created by danish on 3/26/25.
//
#include "card.h"
#include "game_utils.h"

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
    bool gameOver;
    GamePhase gamePhase;
    char lastCommand[20];
    char lastResponse[20];
} GameState;

void gameManager_initGame();
GameState* gameManager_getGameState();
void gameManager_loadDeck(char filePath[100]);
void gameManager_revealDeck();
void gameManager_splitDeck(int splitIndex);
void gameManager_randomShuffleDeck();
void gameManager_saveDeckToFile(char filepath[100]);
void gameManager_quitProgram();
void gameManager_enterPlayMode();
void gameManager_exitPlayMode();
void gameManager_moveCard(Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex);
bool gameManager_isGameOver();


#endif //YUKON_CARDGAME_GAME_MANAGER_H
