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
    srand(time(NULL)); //for random number generation in the gameManager_randomShuffleDeck function
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
    // Check that player is in the startup phase
    if (gameState->gamePhase != StartupPhase){
        strcpy(gameState->lastResponse, "Command not available in the PLAY phase.");
        return;
    }

    if ( strcmp(filePath , "\0") == 0){ //regner med at jeg får filepath som NULL, men kan også ændres til en tom streng
            strcpy(gameState->lastResponse, ("No load file given, new deck created"));
            strcpy(gameState->lastCommand, "LD");
        filePath = "createDeckFile.txt";
    }
    FILE* file = fopen(filePath, "r");
    if (!file) {
        strcpy(gameState->lastResponse, ("Error opening file"));
        strcpy(gameState->lastCommand, "LD");
        return;
    }

    LinkedList* deck = createList(sizeof(Card));

    int deckCheck[52] = {0};  // initialize array to all-zeros
    int cardsAdded = 0;

    char line[4]; // 2 characters + newline + null terminator
    while (fgets(line, sizeof(line), file)) {
        // Ignore lines that are too short
        if (strlen(line) < 2) continue;

        Rank rank = charToRank(line[0]);
        Suit suit = charToSuit(line[1]);

        if (rank == INVALID_RANK) {
            strcpy(gameState->lastResponse, ("invalid rank in load file - Load failed"));
            strcpy(gameState->lastCommand, "LD");
            return;
        }

        if (suit == INVALID_SUIT) {
            strcpy(gameState->lastResponse, ("invalid suit in load file - Load failed"));
            strcpy(gameState->lastCommand, "LD");
            return;
        }

        // Create the card and add it to the deck
        Card* card = createCard(suit, rank, false);
        int indx = rank + suit * 13;
        if (deckCheck[indx] == 0) {
            deckCheck[indx] = 1;
            cardsAdded++;
        } else {
            strcpy(gameState->lastResponse, "Duplicate card found in loaded deck");
            strcpy(gameState->lastCommand, "LD");
            return;
        }
        addNodeToFront(deck, card);
    }

    if (cardsAdded == 52) {
        fclose(file);
        gameState->deck = deck;
        strcpy(gameState->lastResponse, "OK");
        strcpy(gameState->lastCommand, "LD");
    } else {
        strcpy(gameState->lastResponse, ("Not enough cards in loaded deck"));
        strcpy(gameState->lastCommand, "LD");
    }
}
void gameManager_revealDeck(GameState* gameState) {
    // Check that player is in the startup phase
    if (gameState->gamePhase != StartupPhase){
        strcpy(gameState->lastResponse, "Command not available in the PLAY phase.");
        return;
    }

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
    // Check that player is in the startup phase
    if (gameState->gamePhase != StartupPhase){
        strcpy(gameState->lastResponse, "Command not available in the PLAY phase.");
        return;
    }

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
    strcpy(gameState->lastResponse, "OK");

}
void gameManager_randomShuffleDeck(GameState* gameState) {
    // Check that player is in the startup phase
    if (gameState->gamePhase != StartupPhase){
        strcpy(gameState->lastResponse, "Command not available in the PLAY phase.");
        return;
    }

    LinkedList* shuffledDeck = createList(sizeof(Card));
    int cardsLeft = 52;

    while (cardsLeft > 0) {
        int rndIndx = rand() % cardsLeft;

        Node* node = getNode(gameState->deck, rndIndx);
        if (node == NULL || node->data == NULL) {
            printf("Invalid node at index %d\n", rndIndx);
            break;
        }

        Card tmpCard;
        memcpy(&tmpCard, node->data, sizeof(Card));
        deleteNode(gameState->deck, rndIndx);
        addNodeToBack(shuffledDeck, &tmpCard);

        cardsLeft--;
    }

    freeList(gameState->deck);
    gameState->deck = shuffledDeck;
    strcpy(gameState->lastResponse, "Deck shuffled!");
    strcpy(gameState->lastCommand, "SR");
}


void gameManager_saveDeckToFile(GameState* gameState, char filePath[]) {
    // Check that player is in the startup phase
    if (gameState->gamePhase != StartupPhase){
        strcpy(gameState->lastResponse, "Command not available in the PLAY phase.");
        return;
    }

    FILE* file;
    if (strcmp(filePath , "\0") == 0) file = fopen("cards.txt", "w");
    else file = fopen(filePath, "w");

    if (!file) {
        strcpy(gameState->lastResponse, ("Error creating file"));
        strcpy(gameState->lastCommand, "SD");
        return;
    }

    Node* current = gameState->deck->head;

    while (current != NULL) {
        Card* card = (Card*)current->data;

        char * line = cardToString(card);

        fprintf(file, "%s\n", line);

        current = current->nextNode;
    }

    fclose(file);




    strcpy(gameState->lastResponse, ("Saved file!"));
    strcpy(gameState->lastCommand, "SD");

}
void gameManager_quitProgram(GameState* gameState) {
    // Check that player is in the startup phase
    if (gameState->gamePhase != StartupPhase){
        strcpy(gameState->lastResponse, "Command not available in the PLAY phase.");
        return;
    }

}
void gameManager_enterPlayMode(GameState* gameState) {
    // Check that player is in the startup phase
    if (gameState->gamePhase != StartupPhase){
        strcpy(gameState->lastResponse, "Command not available in the PLAY phase.");
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

}
void gameManager_moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex) {
    if (gameState->gamePhase != PlayPhase){
        strcpy(gameState->lastResponse, "Command not available in the STARTUP phase.");
        return;
    }

    LinkedList* srcList;
    LinkedList* dstList;
    Node* srcNode;

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
            strcpy(gameState->lastResponse, "4Move is not valid!");
            return;
        }
        // Validate - Card must be at the bottom of the column when plated onto a foundation
        if (srcNode != srcList->tail){
            strcpy(gameState->lastResponse, "4Move is not valid!");
            return;
        }

        Card* srcCard = srcNode->data;


        if (dstList->tail == NULL){
            // Validate - Card must be ace when put onto empty foundation
            if (srcCard->rank != ACE) {
                strcpy(gameState->lastResponse, "5Move is not valid!");
                return;
            }
        }
        else{
            // Validate - Card must single rank higher and same suit when put onto non-empty foundation
            Card* dstColTailCard = dstList->tail->data;
            if (srcCard->rank-1 != dstColTailCard->rank || srcCard->suit != dstColTailCard->suit) {
                strcpy(gameState->lastResponse, "5Move is not valid!");
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
bool gameManager_isGameOver(GameState* gameState) {
    return false;
}
