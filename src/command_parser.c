//
// Created by danish on 3/26/25.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "game_utils.h"
#include "command_parser.h"

char *readConsoleInput() {
    static char consoleInput[100];
    fgets(consoleInput, sizeof(consoleInput), stdin);

    // Remove the trailing newline if present
    size_t len = strlen(consoleInput);
    if (len > 0 && consoleInput[len - 1] == '\n') {
        consoleInput[len - 1] = '\0';
    }

    return consoleInput;
}

char **str_split(char *a_str, const char *a_delim) {
    // Copy a_str as str_tok modifies the string
    char *str_copy = strdup(a_str);
    if (!str_copy) return NULL;

    // Counting the number of tokens
    char *counting_copy = strdup(str_copy);  // Use the first copy for counting
    if (!counting_copy) {
        free(str_copy);
        return NULL;
    }
    int num_tokens = 0;
    char *token = strtok(counting_copy, a_delim);
    while (token) {
        num_tokens++;
        token = strtok(NULL, a_delim);
    }
    free(counting_copy);

    // Allocate return array of string pointers + 1 for null char
    char **result = malloc((num_tokens + 1) * sizeof(char *));
    if (!result) {
        free(str_copy);
        return NULL;
    }

    // Second pass: extract tokens and allocate memory for each
    token = strtok(str_copy, a_delim);
    int i = 0;
    while (token) {
        result[i] = strdup(token);
        if (!result[i]) {
            // Clean up on failure
            for (int j = 0; j < i; j++) free(result[j]);
            free(result);
            free(str_copy);
            return NULL;
        }
        i++;
        token = strtok(NULL, a_delim);
    }

    // Add NULL terminator to the array
    result[num_tokens] = NULL;

    free(str_copy);
    return result;
}

bool isValidMoveFormat(char str_command[]) {
    // Check if the command is NULL or empty
    if (str_command == NULL || str_command[0] == '\0') {
        return false;
    }

    // Look for the " -> " separator
    const char* arrowPos = strstr(str_command, "->");
    if (arrowPos == NULL) {
        return false;
    }

    // Calculate the lengths of from and to parts
    int fromLength = arrowPos - str_command;
    const char* toStart = arrowPos + 2; // 2 is the length of " -> "

    // Validate they're not empty
    if (fromLength == 0 || toStart[0] == '\0') {
        return false;
    }

    // Extract the from and to parts
    char fromPart[20] = {0};
    char toPart[20] = {0};

    strncpy(fromPart, str_command, fromLength);
    fromPart[fromLength] = '\0';
    strcpy(toPart, toStart);

    // Validate <from> format: can be 'C6:4H', 'C6', or 'F3'

    // Check if it's a foundation source (F3)
    if (fromPart[0] == 'F' && isdigit(fromPart[1])) {
        // Valid foundation source
    }
        // Check if it's a column source with specific card (C6:4H)
    else if (fromPart[0] == 'C' && isdigit(fromPart[1])) {
        // Check for column with specific card
        const char* colonPos = strchr(fromPart, ':');
        if (colonPos != NULL) {
            // Validate card format after colon (e.g., 4H)
            const char* cardStart = colonPos + 1;
            if (strlen(cardStart) < 2) return false;

            // First char should be rank (digit, T, J, Q, K, A)
            char rank = cardStart[0];
            if (!isdigit(rank) &&
                rank != 'T' && rank != 'J' && rank != 'Q' &&
                rank != 'K' && rank != 'A') {
                return false;
            }

            // Second char should be suit (H, D, C, S)
            char suit = cardStart[1];
            if (suit != 'H' && suit != 'D' && suit != 'C' && suit != 'S') {
                return false;
            }
        }
    }
        // If not a valid format
    else {
        return false;
    }

    // Validate <to> format: can be 'C4' or 'F2'
    if ((toPart[0] == 'C' || toPart[0] == 'F') && isdigit(toPart[1])) {
        // Valid destination
    }
    else {
        return false;
    }

    return true;
}

void parseMoveCommand(const char* moveCommand, MVCommand* mvCmd) {
    // Initialize suit and rank to -1 (unspecified) by default
    mvCmd->suit = -1;
    mvCmd->rank = -1;

    // Split the command by "->" to get from and to parts
    char **parts = str_split((char*)moveCommand, "->");

    if (parts != NULL && parts[0] != NULL && parts[1] != NULL) {
        char *fromPart = parts[0];
        char *toPart = parts[1];

        // Extract column/foundation index for 'from'
        if (fromPart[0] == 'C') {
            // Column source
            mvCmd->fromColumnIndex = fromPart[1] - '0' - 1;

            // Check if there's a specific card
            char *colonPos = strchr(fromPart, ':');
            if (colonPos != NULL) {
                // We have a specific card (e.g., C6:4H)
                char *cardStr = colonPos + 1;

                // Set rank and suit using conversion functions
                mvCmd->rank = convertRankToInt(cardStr[0]);
                mvCmd->suit = convertSuitToInt(cardStr[1]);
            }
            // If no colon, suit and rank remain -1 (set by default above)
        } else if (fromPart[0] == 'F') {
            // Foundation source
            mvCmd->fromColumnIndex = fromPart[1] - '0' + 7 - 1;
            // suit and rank remain -1 (set by default above)
        }

        // Extract column/foundation index for 'to'
        if (toPart[0] == 'C') {
            mvCmd->toColumnIndex = toPart[1] - '0' - 1;
        } else if (toPart[0] == 'F') {
            mvCmd->toColumnIndex = toPart[1] - '0' + 7 - 1;
        }
    }

    // Free the parts array
    if (parts != NULL) {
        for (int i = 0; parts[i] != NULL; i++) {
            free(parts[i]);
        }
        free(parts);
    }
}

Command getCommandInput() {
    Command command;
    char *consoleInput = readConsoleInput();
    const char *a_delim = " ";
    char **consoleTokens = str_split(consoleInput, a_delim);

    // token_count number of tokens
    int token_count = 0;
    while (consoleTokens[token_count] != NULL) {
        token_count++;
    }

    char *commandStr = consoleTokens[0];

    command.type = CMD_UNKNOWN; // Unknown command by default

    // Check if command string is equal
    if (strcmp(commandStr, "SW") == 0 && token_count == 1) {
        command.type = CMD_SW;
    } else if (strcmp(commandStr, "SR") == 0 && token_count == 1) {
        command.type = CMD_SR;
    } else if (strcmp(commandStr, "QQ") == 0 && token_count == 1) {
        command.type = CMD_QQ;
    } else if (strcmp(commandStr, "P") == 0 && token_count == 1) {
        command.type = CMD_P;
    } else if (strcmp(commandStr, "Q") == 0 && token_count == 1) {
        command.type = CMD_Q;
    } else if ((strcmp(commandStr, "LD") == 0) && (token_count == 1 || token_count == 2)) {
        command.type = CMD_LD;
        if (token_count == 2) {
            strcpy(command.ld.filePath, consoleTokens[1]);
        } else {
            strcpy(command.ld.filePath, "\0");
        }
    } else if ((strcmp(commandStr, "SI") == 0) && (token_count == 1 || token_count == 2)) {
        command.type = CMD_SI;
        if (token_count == 2) {
            command.si.splitIndex = atoi(consoleTokens[1]);
        } else {
            command.si.splitIndex = -1;
        }
    } else if ((strcmp(commandStr, "SD") == 0) && (token_count == 1 || token_count == 2)) {
        command.type = CMD_SD;
        if (token_count == 2) {
            strcpy(command.ld.filePath, consoleTokens[1]);
        } else {
            strcpy(command.ld.filePath, "\0");
        }
    } else if (isValidMoveFormat(consoleInput) == true) {
        command.type = CMD_MV;
        parseMoveCommand(consoleInput, &command.mv);
    }

    // Free console tokens
    for (int i = 0; consoleTokens[i] != NULL; i++) {
        free(consoleTokens[i]);
    }
    // Free the array itself
    free(consoleTokens);

    return command;
}