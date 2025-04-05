//
// Created by danish on 3/26/25.
//

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "command_parser.h"

char* readConsoleInput() {
    static char consoleInput[100];

    printf("INPUT: ");
    fgets(consoleInput, sizeof(consoleInput), stdin);

    // Remove the trailing newline if present
    size_t len = strlen(consoleInput);
    if (len > 0 && consoleInput[len-1] == '\n') {
        consoleInput[len-1] = '\0';
    }

    return consoleInput;
}

char** str_split(char* a_str, const char* a_delim) {
    // Copy a_str as str_tok modifies the string
    char* str_copy = strdup(a_str);
    if (!str_copy) return NULL;

    // Counting the number of tokens
    char* counting_copy = strdup(str_copy);  // Use the first copy for counting
    if (!counting_copy) {
        free(str_copy);
        return NULL;
    }
    int num_tokens = 0;
    char* token = strtok(counting_copy, a_delim);
    while (token) {
        num_tokens++;
        token = strtok(NULL, a_delim);
    }
    free(counting_copy);

    // Allocate return array of string pointers + 1 for null char
    char** result = malloc((num_tokens + 1) * sizeof(char*));
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

Command getCommandInput() {
    Command command;
    char* consoleInput = readConsoleInput();
    const char* a_delim = " ";
    char** consoleTokens = str_split(consoleInput, a_delim);

    // token_count number of tokens
    int token_count = 0;
    while(consoleTokens[token_count] != NULL) {
        token_count++;
    }
    if (!(token_count > 0 && token_count <= 3)) {
        // return unknown command todo
    }

    char* commandStr = consoleTokens[0];

    // Check if equal
    // One argument commands
    if (token_count > 0 && token_count <= 2) {
        if (strcmp(commandStr, "LD") == 0) {
            command.type = CMD_LD;
            if (token_count == 2) {
                strcpy(command.ld.filePath, consoleTokens[1]);
            }
        } else if (strcmp(commandStr, "SW") == 0) {

        } else if (strcmp(commandStr, "SI") == 0) {

        } else if (strcmp(commandStr, "SR") == 0) {

        } else if (strcmp(commandStr, "SD") == 0) {

        } else if (strcmp(commandStr, "QQ") == 0) {

        } else if (strcmp(commandStr, "P") == 0) {

        } else if (strcmp(commandStr, "Q") == 0) {

        } else if (strcmp(commandStr, "MV") == 0) {

        } else {
            // Unknown command todo
        }
    }

    // Free console tokens
    for (int i = 0; consoleTokens[i] != NULL; i++) {
        free(consoleTokens[i]);
    }
    // Free the array itself
    free(consoleTokens);

    return command;
}