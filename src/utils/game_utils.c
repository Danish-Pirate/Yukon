#include <stddef.h>
#include "game_utils.h"
#include <nfd.h>
#include <stdlib.h>
#include <stdio.h>

nfdu8char_t* getSaveFilePathFromDialog() {
    nfdu8char_t *filePath = NULL;
    nfdu8filteritem_t filters[1] = { { "Deck", "txt" }};
    nfdsavedialogu8args_t args = {0};

    args.filterList = filters;
    args.filterCount = 1;
    args.defaultPath = NULL;
    args.defaultName = "cards.txt";

    // Show the save dialog
    nfdresult_t result = NFD_SaveDialogU8_With(&filePath, &args);

    if (result == NFD_OKAY) {
        // Success - return the path where the file should be saved
        return filePath;
    }
    else if (result == NFD_CANCEL) {
        // User canceled - return empty string
        nfdu8char_t *emptyPath = (nfdu8char_t*)malloc(sizeof(nfdu8char_t));
        if (emptyPath) {
            emptyPath[0] = 0;
        }
        return emptyPath;
    }
    else {
        // Error
        printf("Error: %s\n", NFD_GetError());
        nfdu8char_t *emptyPath = (nfdu8char_t*)malloc(sizeof(nfdu8char_t));
        if (emptyPath) {
            emptyPath[0] = 0;
        }
        return emptyPath;
    }
}
nfdu8char_t *getLoadFilePathFromDialog() {
    nfdu8char_t *nfdPath = NULL;
    nfdu8filteritem_t filters[1] = { { "Card Deck", "txt" }};
    nfdopendialogu8args_t args = {0};
    args.filterList = filters;
    args.filterCount = 1;
    nfdresult_t result = NFD_OpenDialogU8_With(&nfdPath, &args);

    nfdu8char_t *returnPath = NULL;

    if (result == NFD_OKAY) {
        // Success case - copy the path to our own buffer
        size_t len = 0;
        while (nfdPath[len] != 0) len++;

        returnPath = (nfdu8char_t*)malloc((len + 1) * sizeof(nfdu8char_t));
        if (returnPath) {
            for (size_t i = 0; i <= len; i++) {
                returnPath[i] = nfdPath[i];
            }
        }

        NFD_FreePathU8(nfdPath);
    } else {
        // Cancel or error case
        if (result == NFD_ERROR) {
            printf("Error: %s\n", NFD_GetError());
        }

        returnPath = (nfdu8char_t*)malloc(sizeof(nfdu8char_t));
        if (returnPath) {
            returnPath[0] = 0;
        }
    }

    if (!returnPath) {
        static nfdu8char_t fallbackBuffer[1] = {0};
        return fallbackBuffer;
    }

    return returnPath;
}
bool isPointWithinRect(int x, int y, SDL_Rect rect) {
    return
    (x >= rect.x &&
     x <= rect.x + rect.w &&
     y >= rect.y &&
     y <= rect.y + rect.h);
}