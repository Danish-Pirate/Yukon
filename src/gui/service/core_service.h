#pragma once

#include "core/include/yukon_core.h"

void coreService_init();
void coreService_cleanup();

// Game state operations
void coreService_enterPlayMode();
void coreService_exitPlayMode();
const GameView* coreService_getGameView();
bool coreService_isGameWon();
GamePhase coreService_getGamePhase();
GameState* coreService_getGameState();

// Card operations
void coreService_moveCard(Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex);
Card* coreService_getCard(int columnIndex, int cardIndex);
bool coreService_isCardFaceUp(Card* card);
Rank coreService_getCardRank(Card* card);
Suit coreService_getCardSuit(Card* card);
const char* coreService_cardToString(Card* card);

// Deck operations
void coreService_showDeck();
void coreService_saveDeck(const char* filePath);
bool coreService_loadDeck(const char* filePath);
void coreService_toggleShowDeck();
void coreService_shuffleDeck();
void coreService_splitDeck(int splitIndex);
int coreService_getDeckSize();
Card* coreService_getDeckCard(int index);