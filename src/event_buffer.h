#ifndef EVENT_BUFFER_H
#define EVENT_BUFFER_H

typedef struct {
    char *buffer;
    int capacity;
} EventBuffer;

void eventBufferInit(EventBuffer *buffer);

void writeToBuffer(EventBuffer *buffer, const char *message, ...);

#endif