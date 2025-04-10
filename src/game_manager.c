//
// Created by danish on 3/26/25.
//
#include <stdlib.h>
#include "game_manager.h"
#include "card_deck.h"
#include "card.h"

GameState* initGame() {
    GameState* gameState = malloc(sizeof(GameState));

    gameState->gamePhase = StartupPhase;
    gameState->deck = createDeck();

    // Initialize linked lists
    for (int i = 0; i < COLUMNS_SIZE; ++i) {
        gameState->cardColumns[i] = createList(sizeof(Node));
    }
    for (int i = 0; i < PILES_SIZE; ++i) {
        gameState->cardFoundationPiles[i] = createList(sizeof(Node));
    }

    gameState->gameOver = false;

    return gameState;
}

void gameManager_loadDeck(GameState* gameState, char filePath[]) {

}
void gameManager_revealDeck(GameState* gameState) {
    Node* current = gameState->deck->head;
    while (current!=NULL){
        ((Card*) current->data)->isFaceUp = true;
        current = current->nextNode;
    }
}
void gameManager_splitDeck(GameState* gameState, unsigned int splitIndex) {

}
void gameManager_randomShuffleDeck(GameState* gameState) {

}
void gameManager_saveDeckToFile(GameState* gameState, char filepath[]) {

}
void gameManager_quitProgram(GameState* gameState) {

}
void gameManager_enterPlayMode(GameState* gameState) {

}
void gameManager_exitPlayMode(GameState* gameState) {

}
void gameManager_moveCard(GameState* gameState) {

}
bool gameManager_isGameOver(GameState* gameState) {
    return false;
}
