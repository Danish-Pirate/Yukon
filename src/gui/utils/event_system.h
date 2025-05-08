#pragma once

#include <stdbool.h>
#include "model/card.h"
#include "yukon_core.h"

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

typedef struct {
    int buttonId;
    const char* buttonLabel;
} ButtonClickData;

typedef struct {
    Rank rank;
    Suit suit;
    int fromColumnIndex;
    int toColumnIndex;
} CardMoveData;

typedef struct {
    Rank rank;
    Suit suit;
    int columnIndex;
    int cardIndex;
} CardSelectionData;

void eventSystem_init();
void eventSystem_cleanup();
bool eventSystem_subscribe(EventType type, EventCallback callback);
bool eventSystem_unsubscribe(EventType type, EventCallback callback);
void eventSystem_publish(EventType type, void* data);