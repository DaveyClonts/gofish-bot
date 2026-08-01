#include "event_buffer.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void eventBufferInit(EventBuffer *buffer) {
    buffer->capacity = 32;
    buffer->buffer = malloc(buffer->capacity * sizeof(char));
}

void writeToBuffer(EventBuffer *buffer, const char *message, ...) {
    char *reallocPtr = realloc(buffer->buffer, buffer->capacity * sizeof(message));
    if (reallocPtr == NULL) {
        fprintf(stderr, "Error: event buffer realloc failed");
        exit(EXIT_FAILURE);
    }
    buffer->buffer = reallocPtr;

    va_list arguments;
    va_start(arguments, message);
    vsnprintf(buffer->buffer, sizeof(message), message, arguments);
    va_end(arguments);
}