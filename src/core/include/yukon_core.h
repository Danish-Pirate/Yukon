#pragma once

#include <stdbool.h>
#include "data_structures.h" // Include our data structures definitions

typedef enum GamePhase {
    StartupPhase,
    PlayPhase
} GamePhase;

typedef struct GameView {
    bool isGameWon;
    enum GamePhase currentPhase;
} GameView;

typedef enum Rank Rank;
typedef enum Suit Suit;
typedef struct Card Card;
typedef struct GameState GameState;

/**
 * Event types the GUI can subscribe to for game state changes
 */
typedef enum {
    EVENT_SCENE_CHANGE,     /**< Scene has changed */
    EVENT_GAME_INITIALIZED, /**< Game has been initialized */
    EVENT_GAME_WON,         /**< Player has won the game */
    EVENT_PLAY_MODE_ENTER,  /**< Entered play mode */
    EVENT_PLAY_MODE_EXIT,   /**< Exited play mode */
    EVENT_CARD_MOVED,       /**< Card has been moved */
    EVENT_DECK_SHUFFLED,    /**< Deck has been shuffled */
    EVENT_DECK_LOADED_SUCCESS, /**< Deck loaded successfully */
    EVENT_DECK_LOADED_FAILURE, /**< Deck failed to load */
    EVENT_DECK_SAVED,       /**< Deck saved successfully */
    EVENT_DECK_TOGGLED,     /**< Deck face-up/face-down state toggled */
    EVENT_DECK_SPLIT,       /**< Deck split at a specific index */
    EVENT_DECK_SHOWN,
} EventType;

/**
 * Event structure for the event system
 */
typedef struct {
    EventType type; /**< Type of the event */
    void* data;     /**< Data associated with the event */
} Event;

/**
 * Callback function type for event handling
 */
typedef void (*EventCallback)(Event* event);

/* Game initialization and lifecycle */

/**
 * Initialize a new game state.
 *
 * @return Pointer to the newly created game state
 */
GameState* yukon_initGame();

/**
 * Reset the game state to initial values.
 *
 * @param gameState The game state to reset
 */
void yukon_resetGame(GameState* gameState);

/**
 * Clean up and free all resources associated with the game state.
 *
 * @param gameState The game state to clean up
 */
void yukon_cleanup(GameState* gameState);

/* Event system */

/**
 * Initialize the event system.
 */
void yukon_eventSystem_init();

/**
 * Clean up the event system.
 */
void yukon_eventSystem_cleanup();

/**
 * Subscribe to events of a specific type.
 *
 * @param type Type of event to subscribe to
 * @param callback Function to call when event occurs
 * @return true if subscription successful, false otherwise
 */
bool yukon_eventSystem_subscribe(EventType type, EventCallback callback);

/**
 * Unsubscribe from events of a specific type.
 *
 * @param type Type of event to unsubscribe from
 * @param callback Function to remove from subscribers
 * @return true if unsubscription successful, false otherwise
 */
bool yukon_eventSystem_unsubscribe(EventType type, EventCallback callback);

/**
 * Publish an event to all subscribers.
 *
 * @param type Type of event to publish
 * @param data Data associated with the event
 */
void yukon_eventSystem_publish(EventType type, void* data);

/* Game operations */

/**
 * Enter play mode by dealing cards from the deck to the columns.
 *
 * @param gameState The game state
 */
void yukon_enterPlayMode(GameState* gameState);

/**
 * Exit play mode and return to setup mode.
 *
 * @param gameState The game state
 */
void yukon_exitPlayMode(GameState* gameState);

/**
 * Move a card from one column to another.
 *
 * @param gameState The game state
 * @param rank Card rank (or INVALID_RANK to select column tail)
 * @param suit Card suit (or INVALID_SUIT to select column tail)
 * @param fromColumnIndex Source column index
 * @param toColumnIndex Destination column index
 */
void yukon_moveCard(GameState* gameState, Rank rank, Suit suit, int fromColumnIndex, int toColumnIndex);

/**
 * Check if the game has been won.
 *
 * @param gameState The game state
 * @return true if game won, false otherwise
 */
bool yukon_isGameWon(GameState* gameState);

/* Deck operations */

/**
 * Save the current deck to a file.
 *
 * @param gameState The game state
 * @param filePath Path to save the deck file
 */
void yukon_saveDeck(GameState* gameState, const char* filePath);

/**
 * Load a deck from a file.
 *
 * @param gameState The game state
 * @param filePath Path to the deck file
 * @return true if load successful, false otherwise
 */
bool yukon_loadDeck(GameState* gameState, const char* filePath);

/**
 * Make all cards face up
 *
 * @param gameState The game state
 */
void yukon_showDeck(GameState* gameState);

/**
 * Toggle between showing/hiding the deck cards.
 *
 * @param gameState The game state
 */
void yukon_toggleShowDeck(GameState* gameState);

/**
 * Shuffle the deck randomly.
 *
 * @param gameState The game state
 */
void yukon_shuffleDeck(GameState* gameState);

/**
 * Split the deck at a specified index.
 *
 * @param gameState The game state
 * @param splitIndex Index to split at, or -1 for random
 */
void yukon_splitDeck(GameState* gameState, int splitIndex);

/* Game state access */

/**
 * Get the view of the current game state.
 *
 * @param gameState The game state
 * @return Pointer to the game view
 */
const GameView* yukon_getGameView(GameState* gameState);

/**
 * Check if the game has been won from a game view.
 *
 * @param view The game view
 * @return true if game won, false otherwise
 */
bool yukon_getIsGameWon(const GameView* view);

/**
 * Get the current game phase from a game view.
 *
 * @param view The game view
 * @return The current game phase
 */
GamePhase yukon_getGamePhase(const GameView* view);

/* Card access */

/**
 * Get a card from a specific position.
 *
 * @param gameState The game state
 * @param columnIndex Column index
 * @param cardIndex Card index within the column
 * @return Pointer to the card, or NULL if not found
 */
Card* yukon_getCard(GameState* gameState, int columnIndex, int cardIndex);

/**
 * Convert a card to a string representation.
 *
 * @param card The card
 * @return String representation of the card
 */
const char* yukon_cardToString(Card* card);

/**
 * Check if a card is face up.
 *
 * @param card The card
 * @return true if face up, false if face down
 */
bool yukon_isCardFaceUp(Card* card);

/**
 * Get the rank of a card.
 *
 * @param card The card
 * @return The card's rank
 */
Rank yukon_getCardRank(Card* card);

/**
 * Get the suit of a card.
 *
 * @param card The card
 * @return The card's suit
 */
Suit yukon_getCardSuit(Card* card);

/**
 * Get the number of cards in the deck.
 *
 * @param gameState The game state
 * @return Number of cards in the deck
 */
int yukon_getDeckSize(GameState* gameState);

/**
 * Get a card from the deck at a specific index.
 *
 * @param gameState The game state
 * @param index Index of the card in the deck
 * @return Pointer to the card, or NULL if index is invalid
 */
Card* yukon_getDeckCard(GameState* gameState, int index);