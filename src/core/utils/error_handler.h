#pragma once

typedef enum {
    ERROR_NONE,
    ERROR_MEMORY_ALLOCATION,
    ERROR_FILE_IO,
    ERROR_INVALID_PARAMETER,
    ERROR_RENDERER,
    ERROR_TEXTURE,
    ERROR_GAME_LOGIC
} ErrorCode;

void errorHandler_reportError(ErrorCode code, const char* message);
void errorHandler_subscribe(void (*callback)(ErrorCode, const char*));
void errorHandler_unsubscribe(void (*callback)(ErrorCode, const char*));