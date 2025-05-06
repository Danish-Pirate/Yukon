#pragma once

#include <stdbool.h>
#include "model/card.h"
#include "yukon_core.h"

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