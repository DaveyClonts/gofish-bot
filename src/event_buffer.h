#ifndef EVENT_BUFFER_H
#define EVENT_BUFFER_H

typedef struct {
    char *buffer;
    int size;
    int capacity;
} EventBuffer;

void eventBufferInit(EventBuffer *buffer);

void checkEventBufferCapacity(EventBuffer *buffer);

#endif