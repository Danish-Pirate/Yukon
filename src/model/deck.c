#include <string.h>
#include <stdlib.h>
#include "deck.h"
#include "../utils/error_handler.h"

DeckLoadResult loadDeckFromFile(LinkedList **deck, const char *filePath) {
    FILE* file = fopen(filePath, "r");
    if (!file) {
        errorHandler_reportError(ERROR_FILE_IO, "Could not open deck file");
        return LOAD_ERROR_FILE_OPEN;
    }

    LinkedList* newDeck = createList(sizeof(Card));
    if (!newDeck) {
        errorHandler_reportError(ERROR_MEMORY_ALLOCATION, "Failed to allocate memory for deck");
        fclose(file);
        return LOAD_ERROR_FILE_OPEN;
    }

    int deckCheck[52] = {0}, cardsAdded = 0;
    char line[4];
    DeckLoadResult result = LOAD_SUCCESS;

    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) < 2) continue;

        Rank rank = charToRank(line[0]);
        if (rank == INVALID_RANK) {
            errorHandler_reportError(ERROR_INVALID_PARAMETER, "Invalid rank in deck file");
            result = LOAD_ERROR_INVALID_RANK;
            break;
        }

        Suit suit = charToSuit(line[1]);
        if (suit == INVALID_SUIT) {
            errorHandler_reportError(ERROR_INVALID_PARAMETER, "Invalid suit in deck file");
            result = LOAD_ERROR_INVALID_SUIT;
            break;
        }

        int cardIndex = rank + suit * 13;
        if (deckCheck[cardIndex]) {
            errorHandler_reportError(ERROR_GAME_LOGIC, "Duplicate card in deck file");
            result = LOAD_ERROR_DUPLICATE_CARD;
            break;
        }

        Card* card = createCard(suit, rank, false);
        if (!card) {
            errorHandler_reportError(ERROR_MEMORY_ALLOCATION, "Failed to allocate memory for card");
            result = LOAD_ERROR_FILE_OPEN;
            break;
        }

        deckCheck[cardIndex] = 1;
        addNodeToBack(newDeck, card);
        cardsAdded++;
    }

    fclose(file);

    if (result == LOAD_SUCCESS && cardsAdded != 52) {
        errorHandler_reportError(ERROR_GAME_LOGIC, "Incomplete deck in file");
        result = LOAD_ERROR_INCOMPLETE_DECK;
    }

    if (result == LOAD_SUCCESS) {
        if (*deck) freeList(*deck);
        *deck = newDeck;
    } else {
        freeList(newDeck);
    }

    return result;
}

void saveDeckToFile(LinkedList *deck, const char *filePath) {
    FILE* file = fopen(filePath, "w");
    if (!file) {
        errorHandler_reportError(ERROR_FILE_IO, "Could not open file for saving deck");
        return;
    }

    Node* current = deck->head;
    while (current != NULL) {
        Card* card = (Card*)current->data;
        char* line = cardToString(card);
        fprintf(file, "%s\n", line);
        current = current->nextNode;
    }
    fclose(file);
}
void showDeck(LinkedList *deck) {
    if (deck == NULL) return;

    Node* current = deck->head;
    while (current != NULL) {
        Card* card = (Card*)current->data;
        card->isFaceUp = true;
        current = current->nextNode;
    }
}
void toggleShowDeck(LinkedList *deck) {
    if (deck == NULL) return;

    Node* current = deck->head;
    while (current != NULL) {
        Card* card = (Card*)current->data;
        card->isFaceUp = !card->isFaceUp;
        current = current->nextNode;
    }
}
// Fisher-Yates shuffle algorithm
void randomShuffleDeck(LinkedList *deck) {
    if (deck == NULL) return;

    int count = 0;
    Node* current = deck->head;
    while (current != NULL) {
        count++;
        current = current->nextNode;
    }

    if (count <= 1) return; // No need to shuffle a deck with 0 or 1 card

    Node** nodes = (Node**)malloc(count * sizeof(Node*));
    if (nodes == NULL) return;

    current = deck->head;
    for (int i = 0; i < count; i++) {
        nodes[i] = current;
        current = current->nextNode;
    }

    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        void* temp = nodes[i]->data;
        nodes[i]->data = nodes[j]->data;
        nodes[j]->data = temp;
    }

    free(nodes);
}

void splitDeck(LinkedList *deck, int splitIndex) {
    if (deck == NULL || deck->head == NULL || deck->head->nextNode == NULL) {
        return; // Cannot split an empty or single-card deck
    }

    // Validate split index
    int deckSize = 0;
    Node* current = deck->head;
    while (current != NULL) {
        deckSize++;
        current = current->nextNode;
    }

    // Handle negative index (random split)
    if (splitIndex < 0 || splitIndex >= deckSize) {
        splitIndex = rand() % (deckSize - 1) + 1; // Random index between 1 and deckSize-1
    }

    Node* splitNode = deck->head;
    for (int i = 1; i < splitIndex; i++) {
        splitNode = splitNode->nextNode;
    }

    Node* secondPart = splitNode->nextNode;
    if (secondPart == NULL) return; // Cannot split at the last card

    Node* oldTail = deck->tail;

    splitNode->nextNode = NULL;

    secondPart->prevNode = NULL;
    deck->head->prevNode = oldTail;
    oldTail->nextNode = deck->head;

    deck->head = secondPart;
    deck->tail = splitNode;
}
