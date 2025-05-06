#include <stdlib.h>
#include "game.h"
#include "card.h"
#include "deck.h"
#include <stdio.h>
#include <string.h>
#include <utils/linked_list.h>

GameState *initGame() {
    GameState *gameState = malloc(sizeof(GameState));
    if (gameState == NULL) {
        fprintf(stderr, "Failed to allocate memory for gameState\n");
        exit(1);
    }
    resetGame(gameState);
    loadDeckFromFile(&gameState->deck, DECK_FILEPATH);
    return gameState;
}
void resetGame(GameState* gameState){
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
void quitGame(GameState* gameState) {
    gameState->gameWon = true;
}
void enterPlayMode(GameState* gameState) {
    // initialize arrays, to keep track of each stack until max/full is reached
    const unsigned int stackMaxLengths[] = {1,6,7,8,9,10,11};
    unsigned int stackLengths[] = {0,0,0,0,0,0,0};
    Node* deckHead = gameState->deck->head;

    // Iterate through all cards in deck
    while(deckHead != NULL) {
        for (int i = 0; i < COLUMNS_SIZE; i++) {
            // Check if column is not full
            if (stackLengths[i] < stackMaxLengths[i] && deckHead != NULL) {
                // Add reference to the card in the column
                Card* card = (Card*)deckHead->data;

                // Add card reference to column
                addNodeToBack(gameState->cardColumns[i], card);

                // Set isFaceUp property on the card
                card->isFaceUp = stackLengths[i] < i ? false : true;

                stackLengths[i]++;
                deckHead = deckHead->nextNode;
            }
        }
    }

    gameState->gamePhase = PlayPhase;
}
void exitPlayMode(GameState* gameState) {
    LinkedList *deck = gameState->deck;

    for (int i = 0; i < COLUMNS_SIZE; i++) {
        if (gameState->cardColumns[i]) {
            freeListExcludeData(gameState->cardColumns[i]);
        }
    }

    for (int i = 0; i < PILES_SIZE; i++) {
        if (gameState->cardFoundationPiles[i]) {
            freeListExcludeData(gameState->cardFoundationPiles[i]);
        }
    }

    resetGame(gameState);

    gameState->deck = deck;
}
void moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex) {

    LinkedList* srcList;
    LinkedList* dstList;
    Node* srcNode;

    // Validate - Valid indexes
    if (fromColumnIndex < 0 || fromColumnIndex > COLUMNS_SIZE+PILES_SIZE || toColumnIndex < 0 || toColumnIndex > COLUMNS_SIZE+PILES_SIZE){
        return;
    }

    // Validate - the src and dst columns are distinct
    if (fromColumnIndex == toColumnIndex){
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
                return;
            }
            srcNode = current;
        }
        // Fetch tail card if card not specified
        else{
            if (gameState->cardColumns[fromColumnIndex]->tail == NULL){
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
                return;
            }
        }
        else {
            Card *dstColTailCard = dstList->tail->data;
            // Validate - If destination is not empty, card must be single lower rank and different suit
            if (srcCard->rank + 1 != dstColTailCard->rank || srcCard->suit == dstColTailCard->suit) {
                return;
            }
        }
    }
    // Destination is a foundation pile
    else {
        dstList = gameState->cardFoundationPiles[toColumnIndex-COLUMNS_SIZE];
        // Validate - Not valid to move from foundation to other foundation
        if (fromColumnIndex>=COLUMNS_SIZE){
            return;
        }
        // Validate - Card must be at the bottom of the column when plated onto a foundation
        if (srcNode != srcList->tail){
            return;
        }

        Card* srcCard = srcNode->data;


        if (dstList->tail == NULL){
            // Validate - Card must be ace when put onto empty foundation
            if (srcCard->rank != ACE) {
                return;
            }
        }
        else{
            // Validate - Card must single rank higher and same suit when put onto non-empty foundation
            Card* dstColTailCard = dstList->tail->data;
            if (srcCard->rank-1 != dstColTailCard->rank || srcCard->suit != dstColTailCard->suit) {
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
void isGameWon(GameState* gameState) {
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