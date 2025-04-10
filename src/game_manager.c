//
// Created by danish on 3/26/25.
//
#include <stdlib.h>
#include "game_manager.h"
#include "card_deck.h"
#include "card.h"
#include <stdio.h>
#include <string.h>

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
    if (filePath == NULL){ //regner med at jeg får filepath som NULL, men kan også ændres til en tom streng
        gameState->deck = createDeck();
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
    while (current!=NULL){
        ((Card*) current->data)->isFaceUp = true;
        current = current->nextNode;
    }
}
void gameManager_splitDeck(GameState* gameState, unsigned int splitIndex) {

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

}
void gameManager_exitPlayMode(GameState* gameState) {

}
void gameManager_moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex) {

}
bool gameManager_isGameOver(GameState* gameState) {
    return false;
}
