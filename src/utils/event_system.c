#include "event_system.h"

#define MAX_CALLBACKS 32
#define MAX_EVENT_TYPES 16

typedef struct {
    EventCallback callbacks[MAX_CALLBACKS];
    int count;
} CallbackList;

static CallbackList callbackLists[MAX_EVENT_TYPES];
static bool initialized = false;

void eventSystem_init() {
    if (initialized) return;

    for (int i = 0; i < MAX_EVENT_TYPES; i++) {
        callbackLists[i].count = 0;
    }

    initialized = true;
}

void eventSystem_cleanup() {
    // Reset all callback lists
    for (int i = 0; i < MAX_EVENT_TYPES; i++) {
        callbackLists[i].count = 0;
    }

    initialized = false;
}

bool eventSystem_subscribe(EventType type, EventCallback callback) {
    if (!initialized) eventSystem_init();

    if (type >= MAX_EVENT_TYPES) return false;

    CallbackList* list = &callbackLists[type];
    if (list->count >= MAX_CALLBACKS) return false;

    // Check if already subscribed
    for (int i = 0; i < list->count; i++) {
        if (list->callbacks[i] == callback) return true;
    }

    list->callbacks[list->count++] = callback;
    return true;
}

bool eventSystem_unsubscribe(EventType type, EventCallback callback) {
    if (!initialized) return false;

    if (type >= MAX_EVENT_TYPES) return false;

    CallbackList* list = &callbackLists[type];

    for (int i = 0; i < list->count; i++) {
        if (list->callbacks[i] == callback) {
            // Remove by shifting remaining elements
            for (int j = i; j < list->count - 1; j++) {
                list->callbacks[j] = list->callbacks[j + 1];
            }
            list->count--;
            return true;
        }
    }

    return false;
}

void eventSystem_publish(EventType type, void* data) {
    if (!initialized) return;

    if (type >= MAX_EVENT_TYPES) return;

    CallbackList* list = &callbackLists[type];

    Event event = {
            .type = type,
            .data = data
    };

    // Call all registered callbacks
    for (int i = 0; i < list->count; i++) {
        list->callbacks[i](&event);
    }
}