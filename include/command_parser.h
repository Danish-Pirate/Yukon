//
// Created by danish on 3/26/25.
//

#ifndef YUKON_CARDGAME_COMMAND_PARSER_H
#define YUKON_CARDGAME_COMMAND_PARSER_H

#include "card.h"

typedef enum {
    CMD_LD, // Load Deck from file
    CMD_SW, // Reveal cards in deck
    CMD_SI, // Split deck
    CMD_SR, // Randomly shuffle deck
    CMD_SD, // Save deck to file
    CMD_QQ, //n Quit program
    CMD_P,  // Enter play mode
    CMD_Q,  // Exit play mode back to STARTUP mode
    CMD_MV,  // Move card(s)
    CMD_UNKNOWN // Unknown command
} CommandType;

// STARTUP mode commands
typedef struct {
    char filePath[100]; // Filepath will be set to '\0' if no path was provided
} LDCommand;

typedef struct {
    int splitIndex; // If an invalid number of cards was inputted, splitIndex will be set to -1.
} SICommand;

typedef struct {
} SRCommand;

typedef struct {
    char filepath[100]; // Filepath will be set to '\0' if no path was provided
} SDCommand;

typedef struct {
} QQCommand;

typedef struct {
} SWCommand;

typedef struct {
} PCommand;

// PLAY mode commands

typedef struct {
} QCommand;

typedef struct {
} UnknownCommand;

typedef struct {
    char fromColumnIndex; // Index for the column to move the card from
    // The index also includes the foundation piles so (0-10)
    // Suit and rank will be set to -1 if the suit and rank of the card in the fromColumn was not provided in the command
    Suit suit;            // The suit of the card to move
    Rank rank;            // The rank of the card to move

    char toColumnIndex;   // Index for the column to move the card to. Also includes foundation piles
} MVCommand;

typedef struct {
    char* raw_command;
    CommandType type;
    union {
        MVCommand mv;
        LDCommand ld;
        SICommand si;
        SRCommand sr;
        SDCommand sd;
        QQCommand qq;
        PCommand  p;
        QCommand q;
        SWCommand sw;
        UnknownCommand uc;
    };
} Command;

Command getCommandInput();

#endif //YUKON_CARDGAME_COMMAND_PARSER_H
