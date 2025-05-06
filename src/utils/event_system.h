#pragma once

#include <stdbool.h>
#include "../model/card.h"

typedef enum {
    EVENT_SCENE_CHANGE,
    EVENT_CARD_MOVED,
    EVENT_DECK_SHUFFLED,
    EVENT_DECK_LOADED_SUCCESS,
    EVENT_DECK_LOADED_FAILURE,
    EVENT_DECK_SAVED,
    EVENT_DECK_TOGGLED,
    EVENT_DECK_SPLIT,
    EVENT_GAME_WON,
    EVENT_PLAY_MODE_ENTER,
    EVENT_PLAY_MODE_EXIT,
    EVENT_CARD_SELECTED,
    EVENT_CARD_DROPPED,
    EVENT_UI_BUTTON_CLICKED,
    EVENT_GAME_INITIALIZED,
} EventType;

typedef struct {
    EventType type;
    void* data;
} Event;

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