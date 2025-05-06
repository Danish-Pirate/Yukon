#pragma once

#include <stdbool.h>

typedef struct GameState GameState;
typedef struct GameView GameView;
typedef enum GamePhase GamePhase;
typedef enum Rank Rank;
typedef enum Suit Suit;
typedef struct LinkedList LinkedList;
typedef struct Node Node;
typedef struct Card Card;

/* Event types the GUI can subscribe to */
typedef enum {
    EVENT_SCENE_CHANGE,
    EVENT_GAME_INITIALIZED,
    EVENT_GAME_WON,
    EVENT_PLAY_MODE_ENTER,
    EVENT_PLAY_MODE_EXIT,
    EVENT_CARD_MOVED,
    EVENT_CARD_SELECTED,
    EVENT_CARD_DROPPED,
    EVENT_DECK_SHUFFLED,
    EVENT_DECK_LOADED_SUCCESS,
    EVENT_DECK_LOADED_FAILURE,
    EVENT_DECK_SAVED,
    EVENT_DECK_TOGGLED,
    EVENT_DECK_SPLIT,
    EVENT_UI_BUTTON_CLICKED
} EventType;

typedef struct {
    EventType type;
    void* data;
} Event;

typedef void (*EventCallback)(Event* event);

/* Game initialization and lifecycle */
GameState* yukon_initGame();
void yukon_resetGame(GameState* gameState);
void yukon_cleanup(GameState* gameState);

/* Event system */
void yukon_eventSystem_init();
void yukon_eventSystem_cleanup();
bool yukon_eventSystem_subscribe(EventType type, EventCallback callback);
bool yukon_eventSystem_unsubscribe(EventType type, EventCallback callback);
void yukon_eventSystem_publish(EventType type, void* data);

/* Game operations */
void yukon_enterPlayMode(GameState* gameState);
void yukon_exitPlayMode(GameState* gameState);
void yukon_moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex);
bool yukon_isGameWon(GameState* gameState);

/* Deck operations */
void yukon_saveDeck(GameState* gameState, const char* filePath);
bool yukon_loadDeck(GameState* gameState, const char* filePath);
void yukon_toggleShowDeck(GameState* gameState);
void yukon_shuffleDeck(GameState* gameState);
void yukon_splitDeck(GameState* gameState, int splitIndex);

/* Game state access */
const GameView* yukon_getGameView(GameState* gameState);
bool yukon_getIsGameWon(const GameView* view);
GamePhase yukon_getGamePhase(const GameView* view);

/* Card access */
Card* yukon_getCard(GameState* gameState, int columnIndex, int cardIndex);
const char* yukon_cardToString(Card* card);
bool yukon_isCardFaceUp(Card* card);
Rank yukon_getCardRank(Card* card);
Suit yukon_getCardSuit(Card* card);