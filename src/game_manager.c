//
// Created by danish on 3/26/25.
//
#include <stdlib.h>
#include "game_manager.h"
#include "card_deck.h"
#include "card.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

GameState* initGame() {
    GameState* gameState = calloc(1,sizeof(GameState));

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
    if (filePath == NULL){ //regner med at jeg får filepath som NULL, men kan også ændres til en tom streng
        gameState->deck = createDeck();
        return;
    }
    FILE* file = fopen(filePath, "r");
    if (!file) {
        printf("Error opening file");
        return;
    }

    // Ensure the deck is initialized
    if (gameState->deck == NULL) {
        LinkedList* deck = createList(sizeof(Card));
        gameState->deck = deck;
    }


    char line[4]; // 2 characters + newline + null terminator
    while (fgets(line, sizeof(line), file)) {
        // Ignore lines that are too short
        if (strlen(line) < 2) continue;

        Rank rank = charToRank(line[0]);
        Suit suit = charToSuit(line[1]);

        if (rank == INVALID_RANK) {
            printf("Invalid rank: '%c'\n", line[0]);
            continue;
        }

        if (suit == INVALID_SUIT) {
            printf("Invalid suit: '%c'\n", line[1]);
            continue;
        }

        // Create the card and add it to the deck
        Card card = createCard(suit, rank, false);

        addNodeToFront(gameState->deck, &card);
    }


    fclose(file);
}
void gameManager_revealDeck(GameState* gameState) {
    Node* current = gameState->deck->head;
    // Display error if deck is empty
    if (current == NULL){
        strcpy(gameState->lastResponse, "No deck is loaded");
        return;
    }
    while (current!=NULL){
        ((Card*) current->data)->isFaceUp = true;
        current = current->nextNode;
    }
    strcpy(gameState->lastResponse, "OK");
}

// Splits deck into two piles by the splitIndex, and then interleves the second pile into the first,
// and puts the remaining cards at the bottom
void gameManager_splitDeck(GameState* gameState, int splitIndex) {
    // Assume splitIndex is valid??

    // If splitIndex is invalid, generate a random number between 1-51
    if (splitIndex == -1){
        srand(time(NULL));
        splitIndex = (rand() % (DECK_SIZE-1))+1; // Random number between 1-51
    }

    // Create two piles
    Node* firstPile = gameState->deck->head;
    Node* secondPile = getNode(gameState->deck,splitIndex);

    Node* pileDivider=secondPile;


    LinkedList* splitDeck = createList(sizeof(Card));

    // Interleave both piles into splitDeck, until of is empty
    while (firstPile != pileDivider && secondPile != NULL) {
        addNodeToFront(splitDeck,firstPile->data);
        firstPile = firstPile->nextNode;

        addNodeToFront(splitDeck,secondPile->data);
        secondPile = secondPile->nextNode;
    }

    // Add the remaining cards from the FIRST pile to the back of splitdeck
    while (firstPile != pileDivider) {
        addNodeToBack(splitDeck,firstPile->data);
        firstPile = firstPile->nextNode;
    }

    // Add the remaining cards from the SECOND pile to the back of splitdeck
    while (secondPile != NULL) {
        addNodeToBack(splitDeck,secondPile->data);
        secondPile = secondPile->nextNode;
    }

    // Free the old deck
    freeListExcludeData(gameState->deck);
    // Set the split deck to gamestate
    gameState->deck = splitDeck;

}
void gameManager_randomShuffleDeck(GameState* gameState) {


}
void gameManager_saveDeckToFile(GameState* gameState, char filePath[]) {
    FILE* file;
    if (filePath == NULL) file = fopen("cards.txt", "w");
    else file = fopen(filePath, "w");

    if (!file) {
        printf("Error creating the file\n");
        return;
    }

    Node* current = gameState->deck->head;

    while (current != NULL) {
        Card* card = (Card*)current->data;

        char * line = convertCardToString(card);

        fprintf(file, "%s\n", line);

        current = current->nextNode;
    }

    fclose(file);
}
void gameManager_quitProgram(GameState* gameState) {

}
void gameManager_enterPlayMode(GameState* gameState) {
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
}
void gameManager_exitPlayMode(GameState* gameState) {

}
void gameManager_moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex) {

}
bool gameManager_isGameOver(GameState* gameState) {
    return false;
}
