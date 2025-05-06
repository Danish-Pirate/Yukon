#pragma once

#include "../model/game.h"
#include "../utils/event_system.h"

void gameService_init(GameState* initialState);

void gameService_handleDeckOperation(EventType operation, void* data);

typedef struct {
    bool isGameWon;
    GamePhase currentPhase;
} GameView;

const GameView* gameService_getView();