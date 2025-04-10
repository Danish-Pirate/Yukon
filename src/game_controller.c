//
// Created by danish on 3/26/25.
//
#include <stdio.h>
#include "game_controller.h"
#include "game_view.h"
#include "game_manager.h"
#include "command_parser.h"

void processInput(GameState* gameState) {
    Command command = getCommandInput();
    switch(command.type) {
        case CMD_LD: {
            char* filePath = command.ld.filePath;
            gameManager_loadDeck(gameState, filePath);
            break;
        }
        case CMD_SW:
            gameManager_revealDeck(gameState);
            break;
        case CMD_SI: {
            unsigned int splitIndex = command.si.splitIndex;
            gameManager_splitDeck(gameState, splitIndex);
            break;
        }
        case CMD_SR:
            gameManager_randomShuffleDeck(gameState);
            break;
        case CMD_SD: {
            char* filePath = command.sd.filepath;
            gameManager_saveDeckToFile(gameState, filePath);
            break;
        }
        case CMD_QQ:
            gameManager_quitProgram(gameState);
            break;
        case CMD_P:
            gameManager_enterPlayMode(gameState);
            break;
        case CMD_Q:
            gameManager_exitPlayMode(gameState);
            break;
        case CMD_MV: {
            Rank rank = command.mv.rank;
            Suit suit = command.mv.suit;
            int fromColumnIndex = command.mv.fromColumnIndex;
            int toColumnIndex = command.mv.toColumnIndex;
            gameManager_moveCard(gameState, rank, suit, fromColumnIndex, toColumnIndex);
            break;
        }
        default:
            break;
    }
}

void gameLoop() {
    GameState* gameState = initGame();
    initView();

    while (!gameManager_isGameOver(gameState)) {
        updateView(gameState);
        processInput(gameState);
    }
}


