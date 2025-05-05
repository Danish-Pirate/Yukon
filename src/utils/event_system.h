#pragma once

#include <stdbool.h>

typedef enum {
    EVENT_SCENE_CHANGE,
    EVENT_CARD_MOVED,
    EVENT_DECK_SHUFFLED,
    EVENT_GAME_WON,
    EVENT_PLAY_MODE_ENTER,
    EVENT_PLAY_MODE_EXIT,
} EventType;

typedef struct {
    void* data;
    EventType type;
} Event;

typedef void (*EventCallback)(Event* event);
void eventSystem_init();
void eventSystem_cleanup();
bool eventSystem_subscribe(EventType type, EventCallback callback);
bool eventSystem_unsubscribe(EventType type, EventCallback callback);
void eventSystem_publish(EventType type, void* data);