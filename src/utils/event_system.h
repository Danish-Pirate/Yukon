typedef enum {
    EVENT_SCENE_CHANGE,
    EVENT_CARD_MOVED,
    EVENT_DECK_SHUFFLED,
    EVENT_GAME_WON,
    EVENT_PLAY_MODE_ENTER,
    EVENT_PLAY_MODE_EXIT,
    EVENT_CARD_SELECTED,
    EVENT_CARD_DROPPED,
    EVENT_UI_BUTTON_CLICKED,
    EVENT_GAME_INITIALIZED,
} EventType;

// Add these new event data structures
typedef struct {
    int buttonId;
    const char* buttonLabel;
} ButtonClickData;

typedef struct {
    Rank rank;
    Suit suit;
    int columnIndex;
    int cardIndex;
} CardSelectionData;