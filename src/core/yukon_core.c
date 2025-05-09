#include "include/yukon_core.h"
#include "model/game.h"
#include "model/card.h"
#include "model/deck.h"
#include "../gui/utils/event_system.h"
#include <stdlib.h>
#include <string.h>

GameState* yukon_initGame() {
    return initGame();
}

void yukon_resetGame(GameState* gameState) {
    resetGame(gameState);
}

void yukon_cleanup(GameState* gameState) {

}

void yukon_enterPlayMode(GameState* gameState) {
    enterPlayMode(gameState);
}

void yukon_exitPlayMode(GameState* gameState) {
    exitPlayMode(gameState);
}

void yukon_moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex) {
    moveCard(gameState, rank, suit, fromColumnIndex, toColumnIndex);
}

bool yukon_isGameWon(GameState* gameState) {
    isGameWon(gameState);
    return gameState->gameWon;
}

void yukon_saveDeck(GameState* gameState, const char* filePath) {
    saveDeckToFile(gameState->deck, filePath);
}

bool yukon_loadDeck(GameState* gameState, const char* filePath) {
    return loadDeckFromFile(&gameState->deck, filePath) == LOAD_SUCCESS;
}

void yukon_toggleShowDeck(GameState* gameState) {
    toggleShowDeck(gameState->deck);
}

void yukon_shuffleDeck(GameState* gameState) {
    randomShuffleDeck(gameState->deck);
}

void yukon_splitDeck(GameState* gameState, int splitIndex) {
    splitDeck(gameState->deck, splitIndex);
}

// New API functions implementation
const GameView* yukon_getGameView(GameState* gameState) {
    static GameView view;
    view.isGameWon = gameState->gameWon;
    view.currentPhase = gameState->gamePhase;
    return &view;
}

bool yukon_getIsGameWon(const GameView* view) {
    return view->isGameWon;
}

GamePhase yukon_getGamePhase(const GameView* view) {
    return view->currentPhase;
}

Card* yukon_getCard(GameState* gameState, int columnIndex, int cardIndex) {
    if (columnIndex >= 0 && columnIndex < COLUMNS_SIZE) {
        LinkedList* column = gameState->cardColumns[columnIndex];
        Node* node = getNode(column, cardIndex);
        return node ? node->data : NULL;
    }
    return NULL;
}

const char* yukon_cardToString(Card* card) {
    return cardToString(card);
}

bool yukon_isCardFaceUp(Card* card) {
    return card ? card->isFaceUp : false;
}

Rank yukon_getCardRank(Card* card) {
    return card ? card->rank : INVALID_RANK;
}

Suit yukon_getCardSuit(Card* card) {
    return card ? card->suit : INVALID_SUIT;
}
int yukon_getDeckSize(GameState* gameState) {
    if (!gameState || !gameState->deck) {
        return 0;
    }

    int count = 0;
    Node* current = gameState->deck->head;

    while (current != NULL) {
        count++;
        current = current->nextNode;
    }

    return count;
}
void yukon_showDeck(GameState* gameState) {
    return showDeck(gameState->deck);
}
Card* yukon_getDeckCard(GameState* gameState, int index) {
    if (!gameState || !gameState->deck) {
        return NULL;
    }

    Node* current = gameState->deck->head;
    int currentIndex = 0;

    while (current != NULL && currentIndex < index) {
        current = current->nextNode;
        currentIndex++;
    }

    return current ? (Card*)current->data : NULL;
}