#include "event_buffer.h"
#include <stdlib.h>
#include <stdio.h>

void eventBufferInit(EventBuffer *buffer) {
    buffer->size = 0;
    buffer->capacity = 32;
    buffer->buffer = malloc(buffer->capacity * sizeof(char));
}

void checkEventBufferCapacity(EventBuffer *buffer) {
    if (buffer->size >= buffer->capacity) {
        buffer->capacity *= 2;
        char *reallocChar = realloc(buffer->buffer, buffer->capacity * sizeof(char));

        if (reallocChar == NULL) {
            fprintf(stderr, "Error: event buffer realloc failed");
            exit(EXIT_FAILURE);
        }

        buffer->buffer = reallocChar;
    }
}

//TODO: currently broken way to handle size and capacity, size never gets updated. find new solution