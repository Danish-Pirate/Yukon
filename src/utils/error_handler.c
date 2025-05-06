#include "error_handler.h"
#include <stdio.h>

#define MAX_ERROR_CALLBACKS 10

static void (*errorCallbacks[MAX_ERROR_CALLBACKS])(ErrorCode, const char*);
static int callbackCount = 0;

void errorHandler_reportError(ErrorCode code, const char* message) {
    fprintf(stderr, "Error [%d]: %s\n", code, message);

    for (int i = 0; i < callbackCount; i++) {
        errorCallbacks[i](code, message);
    }
}

void errorHandler_subscribe(void (*callback)(ErrorCode, const char*)) {
    if (callbackCount < MAX_ERROR_CALLBACKS) {
        errorCallbacks[callbackCount++] = callback;
    }
}

void errorHandler_unsubscribe(void (*callback)(ErrorCode, const char*)) {
    for (int i = 0; i < callbackCount; i++) {
        if (errorCallbacks[i] == callback) {
            for (int j = i; j < callbackCount - 1; j++) {
                errorCallbacks[j] = errorCallbacks[j + 1];
            }
            callbackCount--;
            return;
        }
    }
}