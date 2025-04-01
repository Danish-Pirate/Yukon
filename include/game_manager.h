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

typedef struct {
    LinkedList cardColumns[COLUMNS_SIZE];
    LinkedList cardFoundationPiles[PILES_SIZE];
    Card deck[DECK_SIZE];        // memory location of the referenced cards in the cardColumns and piles
    bool gameOver;
    char lastCommand[20];
} GameState;

GameState* initGame();
void gameManager_loadDeck(GameState* gameState, char filePath[100]);
void gameManager_revealDeck(GameState* gameState);
void gameManager_splitDeck(GameState* gameState, unsigned int splitIndex);
void gameManager_randomShuffleDeck(GameState* gameState);
void gameManager_saveDeckToFile(GameState* gameState, char filepath[100]);
void gameManager_quitProgram(GameState* gameState);
void gameManager_enterPlayMode(GameState* gameState);
void gameManager_exitPlayMode(GameState* gameState);
void gameManager_moveCard(GameState* gameState);
bool gameManager_isGameOver(GameState* gameState);


#endif //YUKON_CARDGAME_GAME_MANAGER_H
