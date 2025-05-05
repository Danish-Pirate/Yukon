#include <stdlib.h>
#include "game.h"
#include "card.h"
#include "deck.h"
#include <stdio.h>
#include <string.h>

GameState *initGame() {
    GameState *gameState = malloc(sizeof(GameState));
    if (gameState == NULL) {
        fprintf(stderr, "Failed to allocate memory for gameState\n");
        exit(1);
    }
    resetGameState(gameState);
    loadDeckFromFile(&gameState->deck, "createDeckFile.txt");
    return gameState;
}
void resetGameState(GameState* gameState){
    memset(gameState, 0, sizeof(GameState));
    gameState->gamePhase = StartupPhase;
    for (int i = 0; i < COLUMNS_SIZE; ++i) {
        gameState->cardColumns[i] = createList(sizeof(Node));
    }
    for (int i = 0; i < PILES_SIZE; ++i) {
        gameState->cardFoundationPiles[i] = createList(sizeof(Node));
    }
    gameState->gameWon = false;
}
void gameManager_quitProgram(GameState* gameState) {
    // Check that player is in the startup phase
    if (gameState->gamePhase != StartupPhase){
        strcpy(gameState->lastResponse, "Command not available in the PLAY phase.");
        return;
    }
    gameState->gameWon = true;

}
void gameManager_enterPlayMode(GameState* gameState) {
    // Check that player is in the startup phase
    if (gameState->gamePhase != StartupPhase){
        strcpy(gameState->lastResponse, "Command not available in the PLAY phase.");
        return;
    }

    // Validate - Deck not empty
    if (gameState->deck == NULL){
        strcpy(gameState->lastResponse, "Deck is empty, please load a deck first");
        return;
    }

    // initialize arrays, to keep track of each stack until max/full is reached
    const unsigned int stackMaxLengths[] = {1,6,7,8,9,10,11};
    unsigned int stackLengths[] = {0,0,0,0,0,0,0};
    Node* deckHead = gameState->deck->head;
    // Iterate though all cards in deck
    while(deckHead != NULL) {
        for (int i = 0; i < COLUMNS_SIZE; i++) {
            // Check is column is not full
            if (stackLengths[i] < stackMaxLengths[i]) {
                // Add card to column
                addNodeToBack(gameState->cardColumns[i],deckHead->data);

                // Set isFaceUp
                Card* card = gameState->cardColumns[i]->tail->data;
                card->isFaceUp = stackLengths[i] < i ? false : true;;

                stackLengths[i]++;
                deckHead = deckHead->nextNode;
            }
        }
    }
    gameState->gamePhase = PlayPhase;
    strcpy(gameState->lastResponse, "OK");
}
void gameManager_exitPlayMode(GameState* gameState) {
    if (gameState->gamePhase != PlayPhase){
        strcpy(gameState->lastResponse, "Command not available in the STARTUP phase.");
        return;
    }

    // Store deck pointer
    LinkedList *deck = gameState->deck;
    // Store last command
    char lastCommand[100];
    strcpy(lastCommand, gameState->lastCommand);
    // Set all cards to face down
    resetGameState(gameState);
    // Set deck pointer to stored deck
    gameState->deck = deck;
    strcpy(gameState->lastCommand, lastCommand);
    strcpy(gameState->lastResponse, "OK");

}
void gameManager_moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex) {
    if (gameState->gamePhase != PlayPhase){
        strcpy(gameState->lastResponse, "Command not available in the STARTUP phase.");
        return;
    }

    LinkedList* srcList;
    LinkedList* dstList;
    Node* srcNode;

    // Validate - Valid indexes
    if (fromColumnIndex < 0 || fromColumnIndex > COLUMNS_SIZE+PILES_SIZE || toColumnIndex < 0 || toColumnIndex > COLUMNS_SIZE+PILES_SIZE){
        strcpy(gameState->lastResponse, "Move is not valid!");
        return;
    }

    // Validate - the src and dst columns are distinct
    if (fromColumnIndex == toColumnIndex){
        strcpy(gameState->lastResponse, "Move is not valid!");
        return;
    }

    // Source is a column
    if (fromColumnIndex < COLUMNS_SIZE){
        // Card is specified
        if(rank != INVALID_RANK && suit != INVALID_SUIT){
            Node* current = gameState->cardColumns[fromColumnIndex]->head;
            Card* card;
            while (current != NULL) {
                card = current->data;
                if (card->isFaceUp && card->rank == rank && card->suit == suit) {
                    break;
                }
                current = current->nextNode;
            }
            // Validate - The card moved exists at the specified location
            if (current == NULL){
                strcpy(gameState->lastResponse, "Move is not valid!");
                return;
            }
            srcNode = current;
        }
        // Fetch tail card if card not specified
        else{
            if (gameState->cardColumns[fromColumnIndex]->tail == NULL){
                strcpy(gameState->lastResponse, "Move is not valid!");
                return;
            }
            srcNode = gameState->cardColumns[fromColumnIndex]->tail;
        }
        srcList = gameState->cardColumns[fromColumnIndex];
    }
    // Source is a foundation pile
    else {
        srcList = gameState->cardFoundationPiles[fromColumnIndex-COLUMNS_SIZE];
        // Validate - Foundation pile contains a card when used as source
        if (srcList->tail == NULL){
            strcpy(gameState->lastResponse, "Move is not valid!");
            return;
        }
        srcNode = srcList->tail;
    }


    // Destination is a column
    if (toColumnIndex < COLUMNS_SIZE){
        dstList = gameState->cardColumns[toColumnIndex];
        Card* srcCard = srcNode->data;
        if (dstList->tail == NULL) {
            // Validate - If destination is empty, card must be king
            if (srcCard->rank != KING) {
                strcpy(gameState->lastResponse, "Move is not valid!");
                return;
            }
        }
        else {
            Card *dstColTailCard = dstList->tail->data;
            // Validate - If destination is not empty, card must be single lower rank and different suit
            if (srcCard->rank + 1 != dstColTailCard->rank || srcCard->suit == dstColTailCard->suit) {
                strcpy(gameState->lastResponse, "Move is not valid!");
                return;
            }
        }
    }
    // Destination is a foundation pile
    else {
        dstList = gameState->cardFoundationPiles[toColumnIndex-COLUMNS_SIZE];
        // Validate - Not valid to move from foundation to other foundation
        if (fromColumnIndex>=COLUMNS_SIZE){
            strcpy(gameState->lastResponse, "Move is not valid!");
            return;
        }
        // Validate - Card must be at the bottom of the column when plated onto a foundation
        if (srcNode != srcList->tail){
            strcpy(gameState->lastResponse, "Move is not valid!");
            return;
        }

        Card* srcCard = srcNode->data;


        if (dstList->tail == NULL){
            // Validate - Card must be ace when put onto empty foundation
            if (srcCard->rank != ACE) {
                strcpy(gameState->lastResponse, "Move is not valid!");
                return;
            }
        }
        else{
            // Validate - Card must single rank higher and same suit when put onto non-empty foundation
            Card* dstColTailCard = dstList->tail->data;
            if (srcCard->rank-1 != dstColTailCard->rank || srcCard->suit != dstColTailCard->suit) {
                strcpy(gameState->lastResponse, "Move is not valid!");
                return;
            }
        }
    }

    // Reveal bottom card of source if present
    if (srcNode->prevNode != NULL){
        ((Card*)srcNode->prevNode->data)->isFaceUp = true;
    }

    // Move srcNode and subsequent nodes from srcList to tail of dstList
    spliceList(srcList,dstList,srcNode);
    strcpy(gameState->lastResponse, "OK");


}
/*
bool gameManager_isGameOver(GameState* gameState) {
    return gameState->gameOver;
}
 */
/*void gameManager_Save(GameState* gameState, char filepath[100]) {
    FILE* file = fopen(filepath, "w");
    if (!file) {
        strcpy(gameState->lastResponse, ("Error creating file"));
        strcpy(gameState->lastCommand, "SD");
        return;
    }
    for (int i = 0; i < COLUMNS_SIZE; i++) {
        Node* curNode =  gameState->cardColumns[i]->head;
        while (curNode != NULL) {
            Card* curCard = curNode->data;
            fprintf(file, cardToString(curCard));
            fprintf(file, "%d", curCard->isFaceUp);
            curNode = curNode->nextNode;
        } //end of while
        fprintf(file, "\n");
    } //end of column for-loop

    for (int i = 0; i < PILES_SIZE; i++) {
        Node* curNode = gameState->cardFoundationPiles[i]->head;
        while (curNode != NULL) {
            Card* curCard = curNode->data;

        }
    }
}
*/
void gameManager_isGameWon(GameState* gameState) {
    gameState->gameWon = true;

    for (int i = 0; i < PILES_SIZE; i++) {
        LinkedList* pile = gameState->cardFoundationPiles[i];

        // Check win condition for this pile
        if (pile->tail == NULL || ((Card*)pile->tail->data)->rank != KING) {
            gameState->gameWon = false;
            break;
        }
    }
}